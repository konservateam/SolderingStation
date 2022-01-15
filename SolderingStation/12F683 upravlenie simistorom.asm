;v. 01.08.2016
            LIST      p=12F683
            #include  <P12F683.inc> 
            errorlevel	-302		  ;Отключаем сообщения переключения банков 
            __CONFIG  (_MCLRE_OFF & _PWRTE_ON & _CP_OFF & _WDT_ON & _CPD_OFF & _BOD_ON & _IESO_OFF & _FCMEN_OFF & _INTOSCIO)


#define     IN          GPIO,2        ; назначаем порт подключения к PC817
#define     OUT         GPIO,5        ; назначаем порт для управления симистором
#define     fff         .1            ; константа определяющая продолжительность задержки перед началом преобразования АЦП и формирования временной задержки на включение симистора
#define     ggg         .8            ; константа определяющая продолжительность ???????????? ???????? ??? ???????? ????????? ?? ??????????? ???? (???????? ?? ????? 10????)


cblock      0x20
Reg_1                                 ; временный регистр для ПП задержек  
W_TEMP                                ; временный регистр - дубликат W
ST_TEMP                               ; временный регистр - дубликат STATUS
Reg_tmp                               ; временный регистр для хранения результата преобразования АЦП
endc         


; старт программы
            org         0
            goto        START 






; инициализация контроллера
START       
            banksel     OPTION_REG    ; переходим в регистр OPTION_REG
            movlw       b'10001111'   ; отключаем подтягивающие резисторы, включаем предделитель перед WDT 1:128
            movwf       OPTION_REG    ; -//-
            banksel     INTCON        ; переходим в регистр INTCON
            movlw       b'00000000'   ; глобальный запрет прерываний
            movwf       INTCON        ; -//-
            banksel     PIE1          ; переходим в регистр PIE1
            movlw       b'00000000'   ; глобальный запрет периферийных прерываний
            movwf       PIE1          ; -//-
            banksel     OSCCON        ; переходим в регистр OSCCON
            movlw       b'01100101'   ; включаем внутренний генератор на 4 МГц, HFINTOSC стабильна, внутренний тактовый генератор для системных часов
            movwf       OSCCON        ; -//-
            banksel     OSCTUNE       ; переходим в регистр OSCTUNE
            movlw       b'01100101'   ; внутренний генератор работает на калибровочной частоте
            movwf       OSCTUNE       ; -//-
            banksel     GPIO          ; переходим в регистр GPIO
            movlw       b'00010100'   ; настраиваем порт GP2, GP4 на вход, остальные порты на выход!!!!!! (разобраться что за регистр)
            movwf       GPIO          ; -//-
            banksel     TRISIO        ; переходим в регистр TRISIO
            movlw       b'00010100'   ; настраиваем порт GP2, GP4 на вход, остальные порты на выход
            movwf       TRISIO        ; -//-
            banksel     ANSEL         ; переходим в регистр ANSEL
            movlw       b'01011000'   ; Fosc/16, ANS3 аналоговый вход, остальные порты цифровые
            movwf       ANSEL         ; -//-
            banksel     WPU           ; переходим в регистр WPU
            movlw       b'00000000'   ; отключаем подтягивающие резисторы
            movwf       WPU           ; -//-
            banksel     IOC           ; переходим в регистр IOC
            movlw       b'00000000'   ; отключаем прерывания по изменению уровня GP0-GP5
            movwf       IOC           ; -//-
            banksel     T1CON         ; переходим в регистр T1CON
            movlw       b'00000000'   ; отключаем внутренние таймеры
            movwf       T1CON         ; -//-
            banksel     T2CON         ; переходим в регистр T2CON
            movlw       b'00000110'   ; включаем модуль T2CON, коэффициент деления предделителя TMR2 1:16
            movwf       T2CON         ; -//-
            banksel     CMCON0        ; переходим в регистр CMCON0
            movlw       b'00000111'   ; отключаем компаратор
            movwf       CMCON0        ; -//-
            banksel     CMCON1        ; переходим в регистр CMCON1
            movlw       b'00000000'   ; отключаем таймеры, компаратор
            movwf       CMCON1        ; -//-
            banksel     VRCON         ; переходим в регистр VRCON
            movlw       b'00000000'   ; отключаем компаратор
            movwf       VRCON         ; -//-
            banksel     ADCON0        ; переходим в регистр ADCON0
            movlw       b'00001101'   ; включаем модуль АЦП, AN3 - вход, левое выравнивание
            movwf       ADCON0        ; -//- 
            banksel     CCP1CON       ; переходим в регистр CCP1CON
            movlw       b'00000000'   ; отключаем ШИМ
            movwf       CCP1CON       ; -//-          
            banksel     PR2           ; переходим в регистр PR2
            movlw       b'00000000'   ; период для ШИМ        
            movwf       PR2           ; -//-
            bcf         STATUS,RP0    ; переход в банк 0



            bcf         OUT           ; отключаем симистор

go          nop                       ; закольцовка в ожидании перехода через 0
            clrwdt

            btfss       IN            ; проверяем сигнал на входе, пропускаем, если 1
            goto        $-1

            btfsc       IN            ; проверяем сигнал на входе, пропускаем, если 0
            goto        $-1

; временная задержка перед началом преобразования АЦП
            movlw       fff
            movwf       Reg_1
ds          decfsz      Reg_1, F
            goto        ds

            bsf         ADCON0,1      ; включаем преобразование АЦП
            btfsc       ADCON0,1      ; ожидаем завершения
            goto        $-1           ; преобразования
            movfw       ADRESH        ; перепишем результат преобразования
            movwf       Reg_tmp       ; в временный регистр Reg_tmp
            comf        Reg_tmp,F     ; инвертируем Reg_tmp

            bcf         STATUS, Z     ; сбрасываем флаг Z
            movlw       0x00          ; загружаем шаблон для проверки числа 0
            xorwf       Reg_tmp,W     ; сравниваем с регистром Reg_tmp
            btfsc       STATUS, Z     ; проверяем состояние флага Z, пропускаем если 0 (1=совпали данные)
            goto        openn         ; открываем симистор, если результат преобразования АЦП = 255

            bcf         STATUS, Z     ; сбрасываем флаг Z
            movlw       0xFF          ; загружаем шаблон для проверки числа FF (256)
            xorwf       Reg_tmp,W     ; сравниваем с регистром Reg_tmp
            btfsc       STATUS, Z     ; проверяем состояние флага Z, пропускаем если 0 (1=совпали данные)
            goto        go            ; идем на выход, ничего не делаем, если результат преобразования АЦП = 0

; ????????? ???????? ? ??????????? ?? ???????? ?????????? ?? ????? ???
wr2         call        Pause
            decfsz      Reg_tmp, F
            goto        wr2

openn       call        Open100mcs    ; открываем симистор сигналом продолжительностью 100 мкс

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

; включаем симистор на 100 микросекунд
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
