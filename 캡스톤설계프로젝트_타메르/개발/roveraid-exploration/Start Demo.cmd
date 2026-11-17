@echo off
setlocal
cd /d "%~dp0"
if not exist "%~dp0.venv\Scripts\pythonw.exe" (
  echo Missing Python environment. Run setup_environment.ps1 first.
  pause
  exit /b 1
)
start "" "%~dp0.venv\Scripts\pythonw.exe" "%~dp0run_demo.py" --source "%~dp0samples\pedestrians.avi" --autostart
