;v. 01.08.2016
            LIST      p=12F683
            #include  <P12F683.inc> 
            errorlevel	-302		  ;��������� ��������� ������������ ������ 
            __CONFIG  (_MCLRE_OFF & _PWRTE_ON & _CP_OFF & _WDT_ON & _CPD_OFF & _BOD_ON & _IESO_OFF & _FCMEN_OFF & _INTOSCIO)


#define     IN          GPIO,2        ; ��������� ���� ����������� � PC817
#define     OUT         GPIO,5        ; ��������� ���� ��� ���������� ����������
#define     fff         .1            ; ��������� ������������ ����������������� �������� ����� ������� �������������� ��� � ������������ ��������� �������� �� ��������� ���������
#define     ggg         .8            ; ��������� ������������ ����������������� ???????????? ???????? ??? ???????? ????????? ?? ??????????? ???? (???????? ?? ????? 10????)


cblock      0x20
Reg_1                                 ; ��������� ������� ��� �� ��������  
W_TEMP                                ; ��������� ������� - �������� W
ST_TEMP                               ; ��������� ������� - �������� STATUS
Reg_tmp                               ; ��������� ������� ��� �������� ���������� �������������� ���
endc         


; ����� ���������
            org         0
            goto        START 






; ������������� �����������
START       
            banksel     OPTION_REG    ; ��������� � ������� OPTION_REG
            movlw       b'10001111'   ; ��������� ������������� ���������, �������� ������������ ����� WDT 1:128
            movwf       OPTION_REG    ; -//-
            banksel     INTCON        ; ��������� � ������� INTCON
            movlw       b'00000000'   ; ���������� ������ ����������
            movwf       INTCON        ; -//-
            banksel     PIE1          ; ��������� � ������� PIE1
            movlw       b'00000000'   ; ���������� ������ ������������ ����������
            movwf       PIE1          ; -//-
            banksel     OSCCON        ; ��������� � ������� OSCCON
            movlw       b'01100101'   ; �������� ���������� ��������� �� 4 ���, HFINTOSC ���������, ���������� �������� ��������� ��� ��������� �����
            movwf       OSCCON        ; -//-
            banksel     OSCTUNE       ; ��������� � ������� OSCTUNE
            movlw       b'01100101'   ; ���������� ��������� �������� �� ������������� �������
            movwf       OSCTUNE       ; -//-
            banksel     GPIO          ; ��������� � ������� GPIO
            movlw       b'00010100'   ; ����������� ���� GP2, GP4 �� ����, ��������� ����� �� �����!!!!!! (����������� ��� �� �������)
            movwf       GPIO          ; -//-
            banksel     TRISIO        ; ��������� � ������� TRISIO
            movlw       b'00010100'   ; ����������� ���� GP2, GP4 �� ����, ��������� ����� �� �����
            movwf       TRISIO        ; -//-
            banksel     ANSEL         ; ��������� � ������� ANSEL
            movlw       b'01011000'   ; Fosc/16, ANS3 ���������� ����, ��������� ����� ��������
            movwf       ANSEL         ; -//-
            banksel     WPU           ; ��������� � ������� WPU
            movlw       b'00000000'   ; ��������� ������������� ���������
            movwf       WPU           ; -//-
            banksel     IOC           ; ��������� � ������� IOC
            movlw       b'00000000'   ; ��������� ���������� �� ��������� ������ GP0-GP5
            movwf       IOC           ; -//-
            banksel     T1CON         ; ��������� � ������� T1CON
            movlw       b'00000000'   ; ��������� ���������� �������
            movwf       T1CON         ; -//-
            banksel     T2CON         ; ��������� � ������� T2CON
            movlw       b'00000110'   ; �������� ������ T2CON, ����������� ������� ������������ TMR2 1:16
            movwf       T2CON         ; -//-
            banksel     CMCON0        ; ��������� � ������� CMCON0
            movlw       b'00000111'   ; ��������� ����������
            movwf       CMCON0        ; -//-
            banksel     CMCON1        ; ��������� � ������� CMCON1
            movlw       b'00000000'   ; ��������� �������, ����������
            movwf       CMCON1        ; -//-
            banksel     VRCON         ; ��������� � ������� VRCON
            movlw       b'00000000'   ; ��������� ����������
            movwf       VRCON         ; -//-
            banksel     ADCON0        ; ��������� � ������� ADCON0
            movlw       b'00001101'   ; �������� ������ ���, AN3 - ����, ����� ������������
            movwf       ADCON0        ; -//- 
            banksel     CCP1CON       ; ��������� � ������� CCP1CON
            movlw       b'00000000'   ; ��������� ���
            movwf       CCP1CON       ; -//-          
            banksel     PR2           ; ��������� � ������� PR2
            movlw       b'00000000'   ; ������ ��� ���        
            movwf       PR2           ; -//-
            bcf         STATUS,RP0    ; ������� � ���� 0



            bcf         OUT           ; ��������� ��������

go          nop                       ; ����������� � �������� �������� ����� 0
            clrwdt

            btfss       IN            ; ��������� ������ �� �����, ����������, ���� 1
            goto        $-1

            btfsc       IN            ; ��������� ������ �� �����, ����������, ���� 0
            goto        $-1

; ��������� �������� ����� ������� �������������� ���
            movlw       fff
            movwf       Reg_1
ds          decfsz      Reg_1, F
            goto        ds

            bsf         ADCON0,1      ; �������� �������������� ���
            btfsc       ADCON0,1      ; ������� ����������
            goto        $-1           ; ��������������
            movfw       ADRESH        ; ��������� ��������� ��������������
            movwf       Reg_tmp       ; � ��������� ������� Reg_tmp
            comf        Reg_tmp,F     ; ����������� Reg_tmp

            bcf         STATUS, Z     ; ���������� ���� Z
            movlw       0x00          ; ��������� ������ ��� �������� ����� 0
            xorwf       Reg_tmp,W     ; ���������� � ��������� Reg_tmp
            btfsc       STATUS, Z     ; ��������� ��������� ����� Z, ���������� ���� 0 (1=������� ������)
            goto        openn         ; ��������� ��������, ���� ��������� �������������� ��� = 255

            bcf         STATUS, Z     ; ���������� ���� Z
            movlw       0xFF          ; ��������� ������ ��� �������� ����� FF (256)
            xorwf       Reg_tmp,W     ; ���������� � ��������� Reg_tmp
            btfsc       STATUS, Z     ; ��������� ��������� ����� Z, ���������� ���� 0 (1=������� ������)
            goto        go            ; ���� �� �����, ������ �� ������, ���� ��������� �������������� ��� = 0

; ????????? ???????? ? ??????????? ?? ???????? ?????????? ?? ????? ???
wr2         call        Pause
            decfsz      Reg_tmp, F
            goto        wr2

openn       call        Open100mcs    ; ��������� �������� �������� ������������������ 100 ���

            goto        go
            goto        go

;delay = 35 machine cycles
Pause       movlw       ggg
            movwf       Reg_1
wr          decfsz      Reg_1, F
            goto        wr
            clrwdt
            nop
            nop
            return

; �������� �������� �� 100 �����������
Open100mcs  bsf         OUT       
            movlw       .32
            movwf       Reg_1
wr1         decfsz      Reg_1, F
            goto        wr1
            clrwdt
            nop
            bcf         OUT
            return

            end	
