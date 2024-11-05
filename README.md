# MadNES (A crappy NES emulator)

This is a long term side project I was inspired to start after one of the
assignments I did for CMPT 295 (Introduction to Computing Systems) at SFU. In
that project we implemented a few functions to complete a disassembler/simulator
for a subset of RISC-V assembly. I thought it was a pretty fun project, and it
got me thinking: could I adapt/expand this concept to make an emulator for a
game console? Well this is my attempt at exactly that. I'm going to attempt to
implement an NES emulator from scratch in C.

## Goals of this project

This is a very ambitious project, so only time will tell how far I actually get,
but these are my goals for this project in roughly the order I plan to achieve
them in:

- [x] Create a disassembler for 6502 assembly
- [ ] Create C implementations for all 56 instructions to emulate a normal 6502
      program (i.e. no graphics/sound/etc. just a normal CLI application)
- [ ] Begin researching NES graphics and maybe try to get some simple homebrew
      applications running
- [ ] Same thing for audio
- [ ] Figure out how to emulate controller input (probably just from the
      keyboard)
- [ ] Play the OG Super Mario Bros. on a homemade emulator! (probably after a
      LOT of debugging)

I feel fairly confident in my ability to complete the first 2 goals, so I'm
aiming for those for now and will reassess once I feel satisfied with them.
Playing a retail NES game feels _very_ daunting right now, but if I can make it
work that would be pretty awesome! However, if I don't get all the way there,
I'll still see this project as a win as it's going to teach me a lot about
low-level computing, which I think is valuable regardless of what field I
actually end up programming in.

## Non-goals of this project

This is purely an experiment to see if I _can_ do it. I have no plans whatsoever
to make this user-friendly or create releases or anything. There are _plenty_ of
excellent NES emulators out there already that will work 1000000x better than
this project ever will, and that's ok.

As an example of what I mean by not being user friendly, the disassembler part
of this program (all I've got working at the time of writing) requires every
byte of the program to be on its own line and in hexadecimal like this:

```
A9
01
8D
00
02
A9
05
8D
01
02
A9
08
8D
02
02
```

I do not plan on changing this because it works, and figuring out how to read an
NES rom file as it normally is just seems like too much work. I don't plan on
using this regularly if/when it's finished, so I'm ok with having to convert any
actual ROMs that I test into this format. At _most_ I'll make a python script to
do it or something.

I also don't plan on making this cross-platform at all. This is a purely "it
works on my machine" project.

## Usage

If you decide you want to try using this project, here are the steps to
build/run it. The only prerequisites should be `clang` and `make`.

1. Clone the repository (obviously)
2. `cd /path/to/repo/ && make`
3. The binary will be outputted to `./build/bin/nes`.

Just run this file with the appropriate flag and a path to a file that is
formatted like the one above. For example, to disassemble the file I made to
test the `LDA` instruction, run this command:

```
./build/bin/nes -d ./input/d_tests/lda.input
```

and you should get the output

```
LDA  #$01
LDA  $02
LDA  $03,X
LDA  $0405
LDA  $0607,X
LDA  $0809,Y
LDA  ($0a,X)
LDA  ($0b,Y)
```
