yaebumeter
==========

Yet Another EBU Meter.

Fork of [ebumeter](https://kokkinizita.linuxaudio.org/linuxaudio/ebumeter-doc/quickguide.html).


* [New Features](#new-features "")
* [Plans](#plans "")
* [Screenshots](#screenshots "")


![preview](https://github.com/actionless/ebumeter-hidpi/raw/master/screenshots/scale1.png)


## New Features

### HiDPI scale support

Scale support is currently only manual, to set interface scale either
set environment variable or use CLI flag
(CLI flag have higher priority over env var):

```console
ebumeter -z 2
./source/ebumeter -z 1.3
EBUMETER_SCALE=2 ebumeter
EBUMETER_SCALE=1.5 ./source/ebumeter

```

### Connect to jack client via command-line on start

```console
ebumeter -cl "Bose AE2 SoundLink:monitor_FL" -cr "Bose AE2 SoundLink:monitor_FR"
./source/ebumeter -cl 'ATH-M50xBT    :monitor_FL' -cr 'ATH-M50xBT    :monitor_FR'
```


## Plans

 - support for custom themes (and also multiple image scaling)
   - few custom themes inspired by legendary hardware and software
 - set options (like Momentary/Short-term, +9/+18, LU/LUFS, start) via CLI
 - hotkeys
 - use `SDL` and `SDL_image` instead of `Xlib`
 - show ms length of highest TruePeak (and mean/average length of other TruePeaks)
   - add a switch for showing last/highest TruePeak length


## Screenshots

### Original
![preview](https://github.com/actionless/ebumeter-hidpi/raw/master/screenshots/scale1.png)

### x1.5 scale
![preview_scale1.5](https://github.com/actionless/ebumeter-hidpi/raw/master/screenshots/scale1.5.png)

### x2 scale
![preview_scale2](https://github.com/actionless/ebumeter-hidpi/raw/master/screenshots/scale2.png)
