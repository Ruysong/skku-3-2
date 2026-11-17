"""Bounded mission decisions and an explicitly simulated navigation adapter."""
from collections import deque
from dataclasses import dataclass
import math
import time
from .observations import observation_request

UNKNOWN, FREE, OCCUPIED = -1, 0, 100


class MockMap:
    def __init__(self):
        self.width, self.height = 34, 25
        self.truth = [[OCCUPIED] * self.width for _ in range(self.height)]
        for y in range(2, 23):
            for x in range(2, 32):
                if 10 <= y <= 14 or 6 <= x <= 10 or 23 <= x <= 27:
                    self.truth[y][x] = FREE
        # Two physical obstacles in the simulated corridor.
        for x, y in [(17, 10), (17, 11), (18, 10), (25, 17), (26, 17)]:
            self.truth[y][x] = OCCUPIED
        self.grid = [[UNKNOWN] * self.width for _ in range(self.height)]
        self.pose = (7, 12)
        self.path = [self.pose]
        self.route = []
        self.goal = None
        self.visited_goals = []
        self.reveal()

    def neighbors(self, p):
        x, y = p
        return [(a, b) for a, b in [(x-1,y), (x+1,y), (x,y-1), (x,y+1)]
                if 0 <= a < self.width and 0 <= b < self.height]

    def reveal(self):
        x, y = self.pose
        # Synthetic map updates; not a LiDAR raycasting or SLAM claim.
        for b in range(max(0, y-4), min(self.height, y+5)):
            for a in range(max(0, x-4), min(self.width, x+5)):
                self.grid[b][a] = self.truth[b][a]

    def reachable(self):
        parents = {self.pose: None}
        queue = deque([self.pose])
        while queue:
            for n in self.neighbors(queue.popleft()):
                if n not in parents and self.grid[n[1]][n[0]] == FREE:
                    parents[n] = True
                    queue.append(n)
        return set(parents)

    def frontiers(self):
        reachable = self.reachable()
        boundary = {p for p in reachable if any(self.grid[y][x] == UNKNOWN for x, y in self.neighbors(p))}
        groups = []
        while boundary:
            seed = boundary.pop()
            group, queue = [seed], [seed]
            while queue:
                for n in self.neighbors(queue.pop()):
                    if n in boundary:
                        boundary.remove(n)
                        group.append(n)
                        queue.append(n)
            groups.append(group)
        candidates = []
        for group in groups:
            if len(group) < 2:
                continue
            mx, my = sum(p[0] for p in group)/len(group), sum(p[1] for p in group)/len(group)
            p = min(group, key=lambda p: (p[0]-mx)**2 + (p[1]-my)**2)
            distance = math.dist(self.pose, p)
            gain = sum(self.grid[y][x] == UNKNOWN
                       for y in range(max(0,p[1]-4),min(self.height,p[1]+5))
                       for x in range(max(0,p[0]-4),min(self.width,p[0]+5)))
            candidates.append({'id': f'F{p[0]}_{p[1]}', 'cell': list(p),
                               'distance_cells': round(distance, 2), 'unknown_gain': gain,
                               'score': round(gain / (distance+2), 2)})
        return sorted(candidates, key=lambda c: (-c['score'], c['id']))

    def navigate(self, target):
        goal = tuple(target)
        if goal == self.pose:
            return False
        parents, queue = {self.pose: None}, deque([self.pose])
        while queue and goal not in parents:
            p = queue.popleft()
            for n in self.neighbors(p):
                if n not in parents and self.grid[n[1]][n[0]] == FREE:
                    parents[n] = p
                    queue.append(n)
        if goal not in parents:
            return False
        route, p = [], goal
        while p != self.pose:
            route.append(p)
            p = parents[p]
        self.route = route[::-1]
        self.goal = goal
        return True

    def step(self):
        if not self.route:
            return None
        self.pose = self.route.pop(0)
        self.path.append(self.pose)
        self.reveal()
        if not self.route:
            self.visited_goals.append(self.goal)
            self.goal = None
            return 'arrived'
        return 'moving'

    def cancel(self):
        self.route.clear()
        self.goal = None


def world_state(observation, mock_map, mission, source, epoch, recent_people=None, events=None):
    people = observation.get('people', []) if observation else []
    age = time.monotonic() - observation['observed_at'] if observation else None
    return {'mission': mission, 'source': source, 'source_epoch': epoch,
            'navigation_mode': 'MOCK / no physical robot output',
            'map_source': 'synthetic occupancy grid', 'robot_cell': list(mock_map.pose),
            'map_resolution_m': 0.25, 'visited_goal_count': len(mock_map.visited_goals),
            'frontiers': mock_map.frontiers(), 'people': people,
            'recent_person_cues':recent_people or [], 'recent_events':events or [],
            'active_goal_cell':list(mock_map.goal) if mock_map.goal is not None else None,
            'recent_visited_goals':[list(p) for p in mock_map.visited_goals[-5:]],
            'person_coordinates': 'unknown: 2D boxes are not map coordinates',
            'video_age_seconds': round(age, 2) if age is not None else None,
            'frame': observation.get('frame') if observation else None,
            'media_time': observation.get('media_time') if observation else None}


def baseline(state):
    if state['video_age_seconds'] is None or state['video_age_seconds'] > 3:
        return {'action': 'hold', 'target_id': None, 'reason': 'Video is unavailable or stale.'}
    if state['people']:
        p = max(state['people'], key=lambda p: p['confidence'])
        return {'action': 'inspect_person', 'target_id': str(p['track_id']),
                'reason': 'Hold navigation and request visual verification; position is unknown.'}
    if state['frontiers']:
        return {'action': 'explore', 'target_id': state['frontiers'][0]['id'],
                'reason': 'Choose a reachable frontier by unknown-area gain / distance.'}
    return {'action': 'report', 'target_id': None, 'reason': 'No reachable frontier remains in this mock map.'}


def person_targets(state):
    targets = {str(p['track_id']):p for p in state['people']}
    for p in state.get('recent_person_cues',[]):
        age = p.get('last_seen_age_seconds')
        same_epoch = p.get('source_epoch',state['source_epoch'])==state['source_epoch']
        same_pose = p.get('observed_robot_cell',state['robot_cell'])==state['robot_cell']
        if isinstance(age,(int,float)) and not isinstance(age,bool) and 0 <= age <= 15 and same_epoch and same_pose:
            targets.setdefault(str(p['track_id']),p)
    return targets


def validate_decision(data, state):
    if not isinstance(data, dict) or set(data) != {'action', 'target_id', 'reason'}:
        raise ValueError('Expected only action, target_id, reason')
    if not isinstance(data['action'],str) or data['action'] not in {'explore', 'inspect_person', 'report', 'hold'}:
        raise ValueError('Action is not allowed')
    if not isinstance(data['reason'], str) or not 1 <= len(data['reason']) <= 800:
        raise ValueError('Missing or oversized reason')
    target = data['target_id']
    if target is not None and not isinstance(target,str):
        raise ValueError('Target ID must be a string or null')
    if data['action'] == 'explore' and target not in {f['id'] for f in state['frontiers']}:
        raise ValueError('Unknown frontier ID')
    if data['action'] == 'inspect_person' and target not in person_targets(state):
        raise ValueError('Unknown or expired person cue ID')
    if data['action'] in {'hold', 'report'} and target is not None:
        raise ValueError('This action must have a null target')
    return data


@dataclass
class MissionManager:
    status: str = 'IDLE'
    last_observation_request: dict = None

    def apply(self, decision, state, navigation):
        validate_decision(decision, state)
        self.last_observation_request = None
        if state['video_age_seconds'] is None or state['video_age_seconds'] > 3:
            navigation.cancel()
            self.status = 'HOLD: stale video'
            return self.status
        # A current visual candidate always preempts a new exploration request.
        if state['people'] and decision['action'] == 'explore':
            navigation.cancel()
            self.status = 'VERIFY: current person observation takes priority'
            return self.status
        if decision['action'] == 'explore':
            frontier = next(f for f in state['frontiers'] if f['id'] == decision['target_id'])
            self.status = 'MOCK NAVIGATING' if navigation.goto_cell(frontier['cell']) else 'HOLD: unreachable goal'
        else:
            navigation.cancel()
            if decision['action']=='inspect_person':
                person = person_targets(state)[decision['target_id']]
                self.last_observation_request = observation_request(person)
            self.status = {'inspect_person':'VERIFY PERSON / map location unknown',
                           'report':'REPORT READY', 'hold':'HOLD'}[decision['action']]
        return self.status
