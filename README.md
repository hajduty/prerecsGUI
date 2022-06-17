# prerecsGUI

*gui version of https://github.com/gmzorz/prerecs*

![image](https://user-images.githubusercontent.com/60933760/174391395-45c81fe8-342c-4f59-b504-63a4f9651de7.png)

features:
- creates new folder for each video when exporting as png
- ability to select entire folder to scan for video files
- config system, save your settings & queue for later use
- convert image sequence to video

todo: 
- [x] output file to source directory
- [ ] limit ffmpeg cpu usage
- [ ] force cancel ffmpeg encoding
- [x] png/tga to video option

## Requirements

* [Xvid](https://www.xvid.com/download/) (download & install)
* [quicktime](https://support.apple.com/kb/DL837) (download & install)
* [ffmpeg](https://drive.google.com/uc?export=download&id=1ozqMctkULuvVtCogmyZjXGjw7Q9D-Je0) (this file can either be placed in `C:\Windows\System32`, added as an [environment variable](https://video.stackexchange.com/questions/20495/how-do-i-set-up-and-use-ffmpeg-in-windows) or placed in the same directory as `prorec.bat` in order for it to work)
