"""Start/stop presentation console: perception and text AI run together."""
from datetime import datetime
import json
from pathlib import Path
import queue
import threading
import time
import tkinter as tk
from tkinter import filedialog

import cv2
from PIL import Image, ImageTk

from .continuous import ObservationWindow, ReviewScheduler, observation_lines, decision_title
from .mission import MockMap, MissionManager, world_state
from .navigation import MockNavigation
from .observations import DecisionContext
from .reasoner import LocalReasoner
from .recording import Recorder
from .local_model import LocalModelServer

ROOT=Path(__file__).resolve().parents[1]
BG='#101923';PANEL='#192633';FG='#e7eff7';MUTED='#9aacbd';ACCENT='#53d8bd'


class Console:
    def __init__(self,args):
        self.args=args
        self.root=tk.Tk();self.root.title('RoverAid | Live Perception & AI')
        self.root.geometry('1280x800');self.root.minsize(1000,650);self.root.configure(bg=BG)
        self.observations=ObservationWindow(5);self.scheduler=ReviewScheduler(5)
        self.map=MockMap();self.manager=MissionManager();self.navigation=MockNavigation(self.map)
        self.reasoner=LocalReasoner(args.endpoint);self.recorder=Recorder(ROOT/'output')
        self.model_server=LocalModelServer(ROOT,args.endpoint,auto_start=not args.no_auto_model)
        self.backend_queue=queue.Queue();self.backend_ready=False;self.backend_starting=False
        self.backend_retry_at=0
        self.worker=None;self.running=False;self.generation=0;self.revision=0
        self.observation=None;self.decision=None;self.last_request=None
        self.last_error=None
        self.video_queue=queue.Queue(maxsize=1);self.ai_queue=queue.Queue()
        self.source=tk.StringVar(value=str(args.source))
        self.mission=tk.StringVar(value='Search for people, revisit uncertain cues, and report observations.')
        self.source_label=tk.StringVar();self.phase=tk.StringVar(value='Ready. Press Start.')
        self.summary=tk.StringVar(value='Perception evidence accumulates in a rolling 5-second window.')
        self.headline=tk.StringVar(value='Waiting for the first AI decision')
        self.reason=tk.StringVar(value='Press Start. Recent observations are sent to local Qwen automatically.')
        self.result_meta=tk.StringVar(value='No AI response yet')
        self.metrics=tk.StringVar(value='Person detection / tracking / body cues')
        self.footer=tk.StringVar(value='Decision preview / No physical motor output / Results saved automatically')
        self.build();self.update_source_label()
        self.root.protocol('WM_DELETE_WINDOW',self.close)
        self.root.after(60,self.tick)
        self.root.after(100,self.prepare_backend)
        if args.autostart:self.root.after(300,self.start)

    def build(self):
        menu=tk.Menu(self.root);inputs=tk.Menu(menu,tearoff=False)
        inputs.add_command(label='Choose video...',command=self.choose_file)
        inputs.add_command(label='Default webcam',command=lambda:self.choose_source('0'))
        sample=ROOT/'samples/pedestrians.avi'
        inputs.add_command(label='Pedestrian sample',command=lambda:self.choose_source(str(sample)))
        menu.add_cascade(label='Input',menu=inputs)
        view=tk.Menu(menu,tearoff=False);view.add_command(label='Details',command=self.details)
        menu.add_cascade(label='View',menu=view);self.root.configure(menu=menu)
        top=tk.Frame(self.root,bg=BG);top.pack(fill='x',padx=22,pady=(18,12))
        tk.Label(top,text='RoverAid',bg=BG,fg=FG,font=('Segoe UI',24,'bold')).pack(side='left')
        tk.Label(top,text='Perception > Evidence > AI Decision',bg=BG,fg=ACCENT,
                 font=('Segoe UI',11)).pack(side='left',padx=22)
        self.stop_button=tk.Button(top,text='Stop',command=self.stop,bg='#344456',fg=FG,
            relief='flat',font=('Segoe UI',11,'bold'),padx=22,pady=8,state='disabled')
        self.stop_button.pack(side='right')
        self.start_button=tk.Button(top,text='Start',command=self.start,bg=ACCENT,fg=BG,
            relief='flat',font=('Segoe UI',11,'bold'),padx=22,pady=8)
        self.start_button.pack(side='right',padx=8)
        source=tk.Frame(self.root,bg=BG);source.pack(fill='x',padx=22,pady=(0,9))
        tk.Label(source,textvariable=self.source_label,bg=BG,fg=MUTED,font=('Segoe UI',9)).pack(side='left')
        task=tk.Frame(self.root,bg=PANEL);task.pack(fill='x',padx=22,pady=(0,14))
        tk.Label(task,text='Mission',bg=PANEL,fg=ACCENT,font=('Segoe UI',10,'bold'),padx=12).pack(side='left')
        tk.Entry(task,textvariable=self.mission,bg=PANEL,fg=FG,insertbackground=FG,
                 relief='flat',font=('Segoe UI',11)).pack(fill='x',expand=True,ipady=10,padx=(0,12))
        body=tk.Frame(self.root,bg=BG);body.pack(fill='both',expand=True,padx=22)
        body.columnconfigure(0,weight=6,uniform='panel');body.columnconfigure(1,weight=4,uniform='panel')
        body.rowconfigure(0,weight=1)
        left=tk.Frame(body,bg=PANEL);left.grid(row=0,column=0,sticky='nsew',padx=(0,14))
        tk.Label(left,text='Live perception',bg=PANEL,fg=FG,font=('Segoe UI',12,'bold'),
                 anchor='w',padx=14,pady=12).pack(fill='x')
        self.video=tk.Label(left,bg='#0b1119',fg=MUTED,text='Press Start to begin live person detection.',
                            font=('Segoe UI',11));self.video.pack(fill='both',expand=True)
        tk.Label(left,textvariable=self.metrics,bg=PANEL,fg=ACCENT,font=('Segoe UI',10),
                 anchor='w',padx=14,pady=12).pack(fill='x')
        right=tk.Frame(body,bg=BG);right.grid(row=0,column=1,sticky='nsew')
        tk.Label(right,text='Accumulated observations',bg=BG,fg=FG,font=('Segoe UI',12,'bold')).pack(anchor='w')
        self.summary_label=tk.Label(right,textvariable=self.summary,bg=PANEL,fg=FG,
            justify='left',anchor='nw',font=('Segoe UI',10),padx=14,pady=12,wraplength=420)
        self.summary_label.pack(fill='x',pady=(8,15))
        card=tk.Frame(right,bg='#203b3b',padx=16,pady=15);card.pack(fill='both',expand=True)
        tk.Label(card,text='LOCAL QWEN / AUTOMATIC TEXT REASONING',bg='#203b3b',fg=ACCENT,
                 font=('Segoe UI',10,'bold')).pack(anchor='w')
        self.phase_label=tk.Label(card,textvariable=self.phase,bg='#203b3b',fg='#b6d7d0',
                 font=('Segoe UI',10),anchor='w',justify='left',wraplength=400)
        self.phase_label.pack(fill='x',pady=(8,12))
        self.headline_label=tk.Label(card,textvariable=self.headline,bg='#203b3b',fg=FG,
                 font=('Segoe UI',20,'bold'),anchor='w',justify='left',wraplength=400)
        self.headline_label.pack(fill='x',pady=(0,14))
        self.reason_label=tk.Label(card,textvariable=self.reason,bg='#203b3b',fg=FG,
                 font=('Segoe UI',11),anchor='nw',justify='left',wraplength=400)
        self.reason_label.pack(fill='both',expand=True)
        tk.Label(card,textvariable=self.result_meta,bg='#203b3b',fg='#b6d7d0',
                 font=('Segoe UI',9),anchor='w').pack(fill='x',pady=(14,0))
        right.bind('<Configure>',lambda e:self.wrap(e.width))
        tk.Label(right,text='AI decision history',bg=BG,fg=MUTED,font=('Segoe UI',10)).pack(anchor='w',pady=(12,5))
        self.timeline=tk.Text(right,height=4,bg=PANEL,fg=FG,relief='flat',wrap='word',
                             font=('Segoe UI',10),padx=12,pady=8,state='disabled')
        self.timeline.pack(fill='x')
        tk.Label(self.root,textvariable=self.footer,bg=BG,fg=MUTED,font=('Segoe UI',9),
                 anchor='w').pack(fill='x',padx=22,pady=12)

    def wrap(self,width):
        for widget in (self.summary_label,self.phase_label,self.headline_label,self.reason_label):
            widget.configure(wraplength=max(220,width-40))

    def update_source_label(self):
        src=self.source.get()
        self.source_label.set('Source: '+('Default webcam' if src=='0' else Path(src).name)+'   / Change via Input menu')

    def choose_file(self):
        name=filedialog.askopenfilename(filetypes=[('Video','*.mp4 *.avi *.mov *.mkv'),('All files','*.*')])
        if name:self.choose_source(name)

    def choose_source(self,source):
        restart=self.running
        if restart:self.stop()
        self.source.set(source);self.update_source_label()
        if restart:self.start()

    def start(self):
        if self.running:return
        self.running=True;self.revision+=1
        if not self.backend_ready:self.prepare_backend()
        self.start_button.configure(state='disabled');self.stop_button.configure(state='normal')
        self.phase.set('Loading perception model...')
        if self.worker and self.worker.is_alive():
            self.worker.stop_event.set();self.root.after(100,self.wait_worker)
        else:self.launch_worker()

    def wait_worker(self):
        if not self.running:return
        if self.worker and self.worker.is_alive():self.root.after(100,self.wait_worker)
        else:self.launch_worker()

    def launch_worker(self):
        from .app import VideoWorker
        self.generation+=1;self.observations.clear();self.scheduler.restart()
        self.observation=None;self.decision=None;self.last_request=None
        self.headline.set('Waiting for the first AI decision')
        self.reason.set('The latest 5 seconds of observations are analyzed automatically.')
        self.result_meta.set('No AI response yet');self.video_queue=queue.Queue(maxsize=1)
        self.worker=VideoWorker(self.source.get(),ROOT/'models/yolo26n-pose.pt',self.args.conf,
            self.args.imgsz,self.args.device,self.video_queue,self.observations.add,str(self.generation))
        self.worker.start()
        self.recorder.log('continuous_started',source=self.source.get(),review_seconds=5,
                          physical_motor_output=False)

    def state(self,now=None):
        now=time.monotonic() if now is None else now
        snapshot=self.observations.snapshot(now)
        state=world_state(snapshot['latest'],self.map,self.mission.get(),self.source.get(),
            snapshot['source_epoch'] or f'{self.generation}:0',snapshot['recent_people'],snapshot['events'])
        state['observation_window']=snapshot['window']
        state['decision_trigger']={'type':'periodic_observation_review','interval_seconds':5}
        return state

    def request_ai(self,now):
        if not self.backend_ready:return
        state=self.state(now)
        context=DecisionContext(state['source_epoch'],state['mission'],now,self.revision)
        image=self.observation['image'].copy() if self.args.attach_image and self.observation else None
        self.scheduler.begin(now);self.last_request={'state':state,'context':context}
        try:payload=self.reasoner.build_payload(state,image)
        except Exception as error:
            self.ai_queue.put({'error':str(error),'context':context,'state':state})
            return
        self.recorder.log('continuous_ai_request',world_state=state,
                          actual_text_input=payload['messages'][1]['content'][0]['text'])
        def run():
            try:
                decision,meta=self.reasoner.decide(state,image)
                self.ai_queue.put({'decision':decision,'meta':meta,'state':state,'context':context,
                                   'elapsed':time.monotonic()-now})
            except Exception as error:
                self.ai_queue.put({'error':str(error),'context':context,'state':state})
        threading.Thread(target=run,daemon=True).start()

    def finish_ai(self,result,now):
        self.scheduler.finish()
        if not self.running or not result['context'].valid(self.state(now),now,self.revision,max_age=float('inf')):
            self.recorder.log('continuous_result_discarded',reason='stopped, changed session/mission, or expired')
            return
        if 'error' in result:
            self.last_error=result['error'];self.headline.set('AI connection needs attention')
            self.phase.set('AI unavailable. Retrying automatically.')
            self.recorder.log('continuous_ai_error',message=result['error'])
            self.add_timeline('AI request failed / perception continues')
            self.reason.set('No AI response received. Open View > Details for diagnostics.')
            self.footer.set('AI unavailable / Reconnecting locally / No physical motor output')
            self.backend_ready=False;self.backend_retry_at=now+2
            return
        decision=result['decision'];current=self.state(now)
        self.last_error=None
        self.decision=decision
        title=decision_title(decision,result['state'])
        self.headline.set(title);self.reason.set(decision['reason'])
        stamp=datetime.now().strftime('%H:%M:%S')
        target=f"Target #{decision['target_id']} / " if decision.get('target_id') is not None else ''
        self.result_meta.set(f"{target}Qwen / {stamp} / {result['elapsed']:.1f}s")
        if not result['context'].valid(current,now,self.revision):
            self.add_timeline(title+' / historical response: no action applied')
            self.reason.set(decision['reason']+'\n\nEvidence expired. This response is shown for reference only.')
            self.recorder.log('continuous_ai_response',decision=decision,metadata=result['meta'],
                              elapsed=result['elapsed'],application='expired: preview only')
            return
        try:
            self.manager.apply(decision,current,self.navigation)
            self.recorder.log('continuous_proposal_checked',status=self.manager.status,
                              observation_request=self.manager.last_observation_request,physical_motor_output=False)
            blocked=self.manager.status.startswith(('HOLD:','VERIFY:'))
            suffix=' / proposal held: conditions changed' if blocked else ' / proposal ready'
            if blocked:
                self.headline.set('Proposal held for review')
                self.reason.set('AI proposed: '+title+'. '+decision['reason']+'\n\nThe current mission checks do not allow applying this proposal.')
        except ValueError as error:
            self.navigation.cancel();suffix=' / proposal held: evidence changed'
            self.recorder.log('continuous_proposal_rejected',reason=str(error),physical_motor_output=False)
        self.add_timeline(f'{title}{suffix}')
        self.recorder.log('continuous_ai_response',decision=decision,metadata=result['meta'],elapsed=result['elapsed'])
        self.recorder.summary(self.observation,current,decision)
        if self.observation:self.recorder.image('latest.jpg',self.observation['image'])
        self.footer.set('Evidence and AI decisions saved automatically / Preview only / No motor output')

    def add_timeline(self,text):
        self.timeline.configure(state='normal')
        self.timeline.insert('end',f'{datetime.now():%H:%M:%S}  {text}\n')
        if int(self.timeline.index('end-1c').split('.')[0])>60:self.timeline.delete('1.0','2.0')
        self.timeline.see('end');self.timeline.configure(state='disabled')

    def prepare_backend(self):
        if self.backend_starting or self.backend_ready:return
        self.backend_starting=True
        self.phase.set('Starting local AI... / perception can run independently')
        def prepare():
            try:self.backend_queue.put({'ready':self.model_server.ensure_ready()})
            except Exception as error:self.backend_queue.put({'error':str(error)})
        threading.Thread(target=prepare,daemon=True).start()

    def tick(self):
        now=time.monotonic()
        try:
            backend=self.backend_queue.get_nowait();self.backend_starting=False
            if 'error' in backend:
                self.backend_ready=False;self.backend_retry_at=now+8;self.last_error=backend['error']
                self.headline.set('Local AI could not start')
                self.reason.set('Check View > Details. Startup will retry automatically while running.')
                self.recorder.log('local_model_start_error',message=backend['error'])
            else:
                self.backend_ready=True;self.last_error=None
                self.recorder.log('local_model_ready',details=backend['ready'])
                self.footer.set('Local AI ready: '+backend['ready'].get('backend','local server')+' / Preview only / Auto-save on')
        except queue.Empty:pass
        try:
            observation=self.video_queue.get_nowait()
            if self.running:
                if 'error' in observation:
                    self.recorder.log('video_error',message=observation['error']);self.stop()
                    self.phase.set('Video input error');self.footer.set(observation['error'])
                else:
                    self.observation=observation;self.recorder.observation(observation)
                    frame=observation['image'];h,w=frame.shape[:2]
                    scale=min(max(500,self.video.winfo_width())/w,max(250,self.video.winfo_height())/h)
                    preview=cv2.resize(frame,(max(1,int(w*scale)),max(1,int(h*scale))))
                    self.photo=ImageTk.PhotoImage(Image.fromarray(cv2.cvtColor(preview,cv2.COLOR_BGR2RGB)))
                    self.video.configure(image=self.photo,text='')
                    self.metrics.set(f"Visible {len(observation['people'])} / {observation['inference_ms']:.0f}ms inference / video {observation['media_time']:.1f}s")
        except queue.Empty:pass
        try:self.finish_ai(self.ai_queue.get_nowait(),now)
        except queue.Empty:pass
        snapshot=self.observations.snapshot(now)
        self.summary.set(observation_lines(snapshot))
        if self.running:
            if not self.backend_ready and not self.backend_starting and now>=self.backend_retry_at:self.prepare_backend()
            if self.backend_ready and self.scheduler.due(now,snapshot):self.request_ai(now)
            if self.scheduler.busy:
                elapsed=max(0,now-(self.scheduler.last_started or now))
                self.phase.set(f'AI analyzing · {elapsed:.1f}s / perception continues')
            elif self.backend_starting:
                self.phase.set('Starting local AI... / collecting observations')
            elif not self.backend_ready:
                self.phase.set('Local AI unavailable / automatic reconnect pending')
            elif not snapshot['latest']:
                self.phase.set('Preparing live perception...')
            elif now-snapshot['latest']['observed_at']>3:
                self.phase.set('Waiting for fresh video / AI requests paused')
            else:
                wait=self.scheduler.remaining(now,snapshot['first_observed_at'])
                self.phase.set(f'Collecting evidence / next AI review in {wait:.1f}s')
        self.root.after(80,self.tick)

    def stop(self):
        self.running=False;self.revision+=1
        if self.worker:self.worker.stop_event.set()
        self.navigation.cancel();self.phase.set('Stopped / late AI responses will not be applied')
        self.start_button.configure(state='normal');self.stop_button.configure(state='disabled')
        self.recorder.log('continuous_stopped',physical_motor_output=False)
        self.recorder.summary(self.observation,self.state(),self.decision)

    def details(self):
        window=tk.Toplevel(self.root);window.title('RoverAid | Diagnostics');window.geometry('850x650')
        text=tk.Text(window,wrap='word',font=('Consolas',10));text.pack(fill='both',expand=True)
        data={'current_state':self.state(),'last_ai_decision':self.decision,
              'last_error':self.last_error,
              'observation_request':self.manager.last_observation_request,
              'output_directory':str(self.recorder.directory)}
        text.insert('1.0',json.dumps(data,ensure_ascii=False,indent=2));text.configure(state='disabled')

    def close(self):
        self.stop();self.model_server.close();self.recorder.close();self.root.destroy()
