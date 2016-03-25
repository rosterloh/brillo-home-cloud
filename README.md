# brillo-home-cloud

## Setup

* Copy weaved.config.template to weaved.config
* Fill in values from [Weave Developers Console](https://weave.google.com/console/)

## Building

```
$ source envsetup
$ m -j8
```

## Flashing

```
$ adb reboot bootloader
$ provision
$ fastboot reboot
```
