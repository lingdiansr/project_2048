@echo off & title
 
cd /d %~dp0
 
for %%a in (*.wav) do (
 
 ffmpeg -i "%%~sa" -y -acodec libmp3lame -aq 0 "%%~na.mp3"
 
)
 
pause