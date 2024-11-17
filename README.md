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
- [x] Create C implementations for all 56 instructions to emulate a normal 6502
      program (i.e. no graphics/sound/etc.)
- [ ] Write unit tests for all the instructions (probably should've done that
      while implementing them :sweat_smile:)
- [ ] Implement a parser for the iNES ROM format
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

Update: Due to a lack of understanding of how the NES actually works, I assumed
that I could get away with just implementing the 6502 instructions and that
would be all I need. After a bit of preliminary research though, I have found
out that there's more to it than just the 6502, which I probably should've
guessed. Because of this, I will have to implement a parser for the iNES ROM
format, which will make this a lot more "user-friendly" than I originally
planned :sweat_smile:

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
LDA  ($0b),Y
```

Similarly, you can run any of the test input files like this:

```
./build/bin/nes -r ./input/snake.input
```

and you'll get and output like this:

```
-------- Debug Output --------
     A=$1f  X=$ff  Y=$00
      SP=$fb  PC=$0735
          NV-BDIZC
          00110011
------------------------------
```

This output shows the contents of the A, X, and Y registers, the Stack Pointer
(SP), the Program Counter (PC), and the processor status flags, allowing you to
verify that the program at least did _something_.
