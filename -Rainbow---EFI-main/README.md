#### Hello Everyone 


> #### I had this in mind for a long time but I finally got to do it. Originally I was intending to either keep it for myself or sell it, but you know what? Fuck it.

> #### This rainbow spoofer essentially hooks ExitBootServices to get return address to OslFwpKernelSetupPhase1, then it runs sigscan to find OslLoaderBlock and other crap from  there. It hooks IopLoadDriver to perform the spoofing and then unhooks itself.

> #### I have decided not to release the source code yet, because I want to keep it fine for some time until it starts to get flagged. Once it gets flagged, I will publish the source code. The source will be pushed to this GitHub repo.

![r8xNj4f](https://user-images.githubusercontent.com/85826349/126073571-3c35c50f-fe28-4fd2-a640-728b2a5da5f1.png)

![6tS6Yxf](https://user-images.githubusercontent.com/85826349/126073581-8360fde5-c8e8-46b4-bcf7-cf3d8bb8345a.png)

![hLGMoiB](https://user-images.githubusercontent.com/85826349/126073582-93669e82-0712-434c-bf81-99265633e031.png)




## See Video [Click Here](https://streamable.com/dg1tew)


## Usage
Download rainbow.efi and a copy of EDK2 efi shell (direct link). Now follow these steps:

1. Extract downloaded efi shell and rename file Shell.efi (should be in folder UefiShell/X64) to bootx64.efi
2. Format some USB drive to FAT32
3. Create following folder structure

##### Code

```
1.USB:.
2. │   rainbow.efi
3. │
4. └───EFI
5.      └───Boot
6.              bootx64.efi
```
4. Boot from the USB drive
5. An UEFI shell should start, change directory to your USB (FS0 should be the USB since we are booting from it) and list files

##### Code

```
1.FS0:
2.ls
```

6. You should see file rainbow.efi, if you do, load it

##### Code

```
load rainbow.efi
```

7. Now you should see output from rainbow. If it was successful, exit and boot into Windows (change to Windows boot media - usually FS1 - and run \EFI\Boot\bootx64.efi)

#### Download

*Version 0.0.1*

- Target Windows build: 20H2 (19042.xxx)
- Download: [Click](https://github.com/SmurfsCC/-Rainbow---EFI/releases/tag/Rainbow)


### Note 

##### to mods: The URL (rainbow.tulach.cc) is just a redirect to that GitHub page. The page itself is my blog (other people have been also linking their blogs so I presume it's fine?). Originally it was intended as a webpage for it, and now I am lazy to rebuild since I am already on my laptop (gonna be midnight). If it's triggering you, I will rebuild it and retake the screenshots and video tomorrow.

### Note

##### to mods 2: The compiled file uses simple compile time XOR for text. I did this to prevent people from simply byte-patching it. If you need PDB files to check it (or even the source) then I will provide it to you.

**EDIT**

###### Source code has been released. Please check out the GitHub repo. The source code is provided "as-is" and I will not be providing any support for that.

              
