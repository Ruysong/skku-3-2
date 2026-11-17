"""GUI-friendly entrypoint, including diagnostics when launched with pythonw."""
from pathlib import Path
import sys
import traceback

ROOT=Path(__file__).resolve().parent

if __name__ == '__main__':
    log=None
    if sys.stdout is None or sys.stderr is None:
        (ROOT/'runtime').mkdir(exist_ok=True)
        log=(ROOT/'runtime/application.log').open('a',encoding='utf-8',buffering=1)
        if sys.stdout is None:sys.stdout=log
        if sys.stderr is None:sys.stderr=log
    try:
        from roveraid.app import main
        main()
    except Exception:
        traceback.print_exc()
        import tkinter as tk
        from tkinter import messagebox
        error_root=tk.Tk();error_root.withdraw()
        messagebox.showerror('RoverAid startup error',
            'The application could not start. Check runtime/application.log or run start_demo.ps1 for details.')
        error_root.destroy()
        raise
    finally:
        if log:log.close()
