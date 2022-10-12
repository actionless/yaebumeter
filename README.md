ebumeter-hidpi
==============

Fork of [ebumeter](https://kokkinizita.linuxaudio.org/linuxaudio/ebumeter-doc/quickguide.html).




## New features

### HiDPI scale support

Scale support is currently only manual, to use run:

```console
EBUMETER_SCALE=2 ebumeter
EBUMETER_SCALE=1.5 ./source/ebumeter
```

### Connect to jack client via command-line on start

```console
ebumeter -cl "Bose AE2 SoundLink:monitor_FL" -cr "Bose AE2 SoundLink:monitor_FR"
./source/ebumeter -cl 'ATH-M50xBT    :monitor_FL' -cr 'ATH-M50xBT    :monitor_FR'
```


## Some screenshots

### Original
![preview](https://github.com/actionless/ebumeter-hidpi/raw/master/screenshots/scale1.png)

### x1.5 scale
![preview_scale1.5](https://github.com/actionless/ebumeter-hidpi/raw/master/screenshots/scale1.5.png)

### x2 scale
![preview_scale2](https://github.com/actionless/ebumeter-hidpi/raw/master/screenshots/scale2.png)
