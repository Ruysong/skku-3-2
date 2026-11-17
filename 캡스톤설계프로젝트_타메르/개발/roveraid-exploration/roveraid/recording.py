from datetime import datetime
from pathlib import Path
import json
import cv2


class Recorder:
    def __init__(self, root):
        self.directory = Path(root) / datetime.now().strftime('%Y%m%d-%H%M%S-%f')
        self.directory.mkdir(parents=True, exist_ok=True)
        self.events = (self.directory / 'events.jsonl').open('w', encoding='utf-8')
        self.saved = set()

    def log(self, event, **details):
        row = {'time':datetime.now().astimezone().isoformat(timespec='milliseconds'),
               'event':event, **details}
        self.events.write(json.dumps(row, ensure_ascii=False) + '\n')
        self.events.flush()
        return row

    def image(self, name, frame):
        ok, encoded = cv2.imencode('.jpg', frame)
        if ok:
            encoded.tofile(str(self.directory / name))

    def observation(self, observation):
        for person in observation['people']:
            session = observation.get('session', 'headless')
            key = (session, person['track_id'])
            if key not in self.saved:
                self.saved.add(key)
                self.image(f"session-{session}-track-{person['track_id']}-first.jpg", observation['image'])
                self.log('person_first_seen', session=session, person=person)

    def summary(self, observation, state, decision):
        data = {'perception_source':state['source'], 'map_source':state['map_source'],
                'physical_motor_output':False, 'world_state':state,
                'tracks':observation.get('history',[]) if observation else [],
                'latest_decision':decision,
                'note':'Track count is not a verified count of unique people. Posture is a geometry heuristic.'}
        (self.directory/'summary.json').write_text(json.dumps(data,ensure_ascii=False,indent=2),encoding='utf-8')

    def close(self):
        self.events.close()
