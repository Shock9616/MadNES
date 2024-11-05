first:
ADC #$01

BCC first
BCS second

second:
LDA #$00

BEQ third

LDA #$69

third:
ADC #$FF

BMI fourth

LDA #$42

fourth:
ADC #$06

BNE fifth

LDA #$21

fifth:
ADC #$06
BPL sixth

LDA #$FF

sixth:
ADC #$17
