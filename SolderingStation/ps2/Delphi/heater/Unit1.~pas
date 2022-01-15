unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, Buttons, ComCtrls, CheckLst, Grids, ValEdit,
  CPort;

type
  TForm1 = class(TForm)
    Image1: TImage;
    UpDown1: TUpDown;
    Edit1: TEdit;
    Timer1: TTimer;
    UpDown2: TUpDown;
    Edit2: TEdit;
    Image2: TImage;
    Edit3: TEdit;
    UpDown3: TUpDown;
    Edit4: TEdit;
    UpDown4: TUpDown;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton5: TSpeedButton;
    StringGrid1: TStringGrid;
    CheckBox1: TCheckBox;
    CheckBox2: TCheckBox;
    SpeedButton3: TSpeedButton;
    ListBox1: TListBox;
    ComPort1: TComPort;
    ComboBox1: TComboBox;
    Button1: TButton;
    procedure FormCreate(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
    procedure SpeedButton2Click(Sender: TObject);
    procedure StringGrid1DrawCell(Sender: TObject; ACol, ARow: Integer;
      Rect: TRect; State: TGridDrawState);
    procedure SpeedButton5Click(Sender: TObject);
    procedure SpeedButton3Click(Sender: TObject);
    procedure ComPort1RxChar(Sender: TObject; Count: Integer);
    procedure Button1Click(Sender: TObject);
  private
    procedure DrawGraph;
    procedure DrawPane;
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}
uses IniFiles, RegExpr;

var
  BottomBuf : Array[1..600] of Integer;
  BottomBufLen : Integer;
  TopBuf : Array[1..600] of Integer;
  TopBufLen : Integer;
  Bitmap1, Bitmap2 : TBitmap;
  TopHeaterState, BottomHeaterState: Integer;
  T : String;

procedure TForm1.DrawGraph;
var
  I, S : Integer;
begin
  Image1.Canvas.Brush.Style := bsSolid;
  Image1.Canvas.Brush.Color := clBlack;
  Image1.Canvas.FillRect(Rect(0,0,640,550));

  Image1.Canvas.Pen.Width := 1;
  Image1.Canvas.Font.Color := clYellow;
  Image1.Canvas.Brush.Color := clBlack;
  Image1.Canvas.Pen.Color := $00202020;
  Image1.Canvas.Pen.Style := psDot;
  for I := 0 to 12 do
  begin
    S := 600 - I*50;
    Image1.Canvas.TextOut(602 - S, 505, IntToStr(S));
    Image1.Canvas.MoveTo(601 - S, 2);
    Image1.Canvas.LineTo(601 - S, 499);
  end;
  for I := 0 to 12 do
  begin
    S := 250 - I*20;
    Image1.Canvas.TextOut(602, 504 - S*2, IntToStr(S));
    Image1.Canvas.MoveTo(2, 504 - S*2);
    Image1.Canvas.LineTo(598, 504 - S*2);
  end;

  Image1.Canvas.Pen.Style := psSolid;
  Image1.Canvas.Pen.Color := clRed;
  Image1.Canvas.Pen.Width := 1;
  Image1.Canvas.MoveTo(2,2);
  Image1.Canvas.LineTo(2,501);
  Image1.Canvas.LineTo(601,501);
  Image1.Canvas.LineTo(601,2);
  Image1.Canvas.LineTo(2,2);

  Image1.Canvas.Pen.Width := 3;
  Image1.Canvas.Pen.Color := clYellow;
  Image1.Canvas.MoveTo(599, 504 - BottomBuf[1]*2);
  for I := 2 to BottomBufLen do
  begin
    Image1.Canvas.LineTo(600-I, 504 - BottomBuf[I]*2);
  end;
  Image1.Canvas.Pen.Color := clRed;
  Image1.Canvas.MoveTo(599, 504 - TopBuf[1]*2);
  for I := 2 to TopBufLen do
  begin
    Image1.Canvas.LineTo(600-I, 504 - TopBuf[I]*2);
  end;
end;

procedure TForm1.DrawPane;
begin
  Image2.Canvas.Brush.Style := bsSolid;
  Image2.Canvas.Brush.Color := clBlack;
  Image2.Canvas.FillRect(Rect(0,0,Image2.Width,Image2.Height));
  if BottomHeaterState>0 then
     Image2.Canvas.Draw(0,80,BitMap1)
  else
  begin
    Image2.Canvas.Brush.Color := clDkGray;
    Image2.Canvas.FillRect(Rect(17,100,374,120));
  end;

  Image2.Canvas.Brush.Color := clWhite;
  Image2.Canvas.FillRect(Rect(17,80,374,85)); //Board
  Image2.Canvas.FillRect(Rect(137,72,254,78)); //Chip
  Image2.Canvas.FillRect(Rect(177,68,214,78));

  if TopHeaterState>0 then
    Image2.Canvas.Draw(100,5,BitMap2)
  else
  begin
    Image2.Canvas.Brush.Color := clDkGray;
    Image2.Canvas.FillRect(Rect(140,27,252,43));
  end;

  Image2.Canvas.Brush.Color := clWhite;
  Image2.Canvas.Pen.Color := clWhite;
  Image2.Canvas.Pen.Width := 3;
  Image2.Canvas.Ellipse(75, 75, 81, 81);
  Image2.Canvas.MoveTo(78, 78);
  Image2.Canvas.LineTo(48, 48);

  Image2.Canvas.Ellipse(231, 66, 237, 72);
  Image2.Canvas.MoveTo(234, 69);
  Image2.Canvas.LineTo(324, 58);

  Image2.Canvas.Brush.Color := clBlack;
  Image2.Canvas.Font.Size := 24;
  Image2.Canvas.Font.Color := clYellow;
  Image2.Canvas.TextOut(10, 10, Edit1.Text+'°');
  Image2.Canvas.Font.Color := clRed;
  Image2.Canvas.TextOut(344, 40, Edit2.Text+'°');
  Image2.Canvas.Font.Color := clWhite;
  Image2.Canvas.TextOut(260, 10, IntToStr(TopHeaterState)+'%');
  Image2.Canvas.TextOut(380, 90, IntToStr(BottomHeaterState)+'%');
end;


procedure TForm1.FormCreate(Sender: TObject);
begin
  Bitmap1 := TBitMap.Create;
  Bitmap1.LoadFromResourceName(HInstance,'B_ON');
  Bitmap2 := TBitMap.Create;
  Bitmap2.LoadFromResourceName(HInstance,'T_ON');
  BottomBufLen := 0;
  TopBufLen := 0;
  Doublebuffered:=true;
  BottomHeaterState := 0;
  TopHeaterState := 0;
  StringGrid1.ColWidths[0] := 222;
  StringGrid1.ColWidths[1] := 222;
  StringGrid1.ColWidths[2] := 1;
  StringGrid1.Cells[0,0] := 'Condition';
  StringGrid1.Cells[1,0] := 'Action';
  StringGrid1.Cells[2,0] := '';
  DrawGraph;
  DrawPane;
  EnumComPorts(Combobox1.Items);
  Combobox1.ItemIndex := 0;
end;

procedure TForm1.Timer1Timer(Sender: TObject);

  function ParseExpression (E : String) : Integer;
    function ParseSubExpression (L, R : Integer; S : String) : Boolean;
    begin
      Result := False;
      if S = '<' then Result := (L < R);
      if S = '<=' then Result := (L <= R);
      if S = '>' then Result := (L > R);
      if S = '>=' then Result := (L >= R);
      if S = '=' then Result := (L = R);
      if S = '<>' then Result := (L <> R);
    end;
  var
    r1, r2 : TRegExpr;
    TL, TR : Integer;
    S : String;
  begin
    r1 := TRegExpr.Create;
    r2 := TRegExpr.Create;
    r1.Expression := '([^&]+)&?';
    S := '';
    Result := 1;
    if r1.Exec(E) then
    begin
      r2.Expression := '^((T_BOTTOM|T_TOP|USE_TOP)(<|>|=|<=|>=|<>)([0-9]+))$';
      if r2.Exec(r1.Match[1]) then
      begin
         if r2.Match[2] = 'T_BOTTOM' then
         begin
            TL := StrToInt(Edit1.Text);
            TR := StrToInt(r2.Match[4]);
            if not ParseSubExpression(TL, TR, r2.Match[3]) then Result := 0;
         end;
         if r2.Match[2] = 'T_TOP' then
         begin
            TL := StrToInt(Edit2.Text);
            TR := StrToInt(r2.Match[4]);
            if not ParseSubExpression(TL, TR, r2.Match[3]) then Result := 0;
         end;
         if r2.Match[2] = 'USE_TOP' then
         begin
            if CheckBox1.Checked then TL := 1 else TL := 0;
            TR := StrToInt(r2.Match[4]);
            if not ParseSubExpression(TL, TR, r2.Match[3]) then Result := 0;
         end;
      end else Result := -1;
      while r1.ExecNext do
      begin
        if r2.Exec(r1.Match[1]) then
        begin
           if r2.Match[2] = 'T_BOTTOM' then
           begin
              TL := StrToInt(Edit1.Text);
              TR := StrToInt(r2.Match[4]);
              if not ParseSubExpression(TL, TR, r2.Match[3]) then Result := 0;
           end;
           if r2.Match[2] = 'T_TOP' then
           begin
              TL := StrToInt(Edit2.Text);
              TR := StrToInt(r2.Match[4]);
              if not ParseSubExpression(TL, TR, r2.Match[3]) then Result := 0;
           end;
           if r2.Match[2] = 'RULE_TOP' then
           begin
              if CheckBox1.Checked then TL := 1 else TL := 0;
              TR := StrToInt(r2.Match[4]);
              if not ParseSubExpression(TL, TR, r2.Match[3]) then Result := 0;
           end;
        end else Result := -1;
      end;
    end else Result := -1;
    r1.Free;
    r2.Free;
  end;

  function ParseAction(A : String) : Integer;
  var
    r1, r2 : TRegExpr;
    TR : Integer;
  begin
    r1 := TRegExpr.Create;
    r2 := TRegExpr.Create;
    r1.Expression := '([^&]+)&?';
    Result := 1;
    if r1.Exec(A) then
    begin
      r2.Expression := '^((P_BOTTOM|P_TOP|USE_TOP)(:=)([0-9]+))$';
      if r2.Exec(r1.Match[1]) then
      begin
        if r2.Match[2] = 'P_BOTTOM' then
        begin
          TR := StrToInt(r2.Match[4]);
          if (Checkbox2.Checked) and (TR <= 100) and (TR >= 0) then Edit4.Text := IntToStr(TR);
        end;
        if r2.Match[2] = 'P_TOP' then
        begin
          TR := StrToInt(r2.Match[4]);
          if (Checkbox2.Checked) and (TR <= 100) and (TR >= 0) then Edit3.Text := IntToStr(TR);
        end;
        if r2.Match[2] = 'RULE_TOP' then
        begin
          TR := StrToInt(r2.Match[4]);
          if (Checkbox2.Checked) and (TR = 1) then CheckBox1.Checked := True;
          if (Checkbox2.Checked) and (TR = 0) then CheckBox1.Checked := False;
        end;
      end else Result := -1;
      while r1.ExecNext do
      begin
        if r2.Exec(r1.Match[1]) then
        begin
          if r2.Match[2] = 'P_BOTTOM' then
          begin
             TR := StrToInt(r2.Match[4]);
             if (Checkbox2.Checked) and (TR <= 100) and (TR >= 0) then Edit4.Text := IntToStr(TR);
          end;
          if r2.Match[2] = 'P_TOP' then
          begin
            TR := StrToInt(r2.Match[4]);
            if (Checkbox2.Checked) and (TR <= 100) and (TR >= 0) then Edit3.Text := IntToStr(TR);
          end;
          if r2.Match[2] = 'USE_TOP' then
          begin
            TR := StrToInt(r2.Match[4]);
            if (Checkbox2.Checked) and (TR = 1) then CheckBox1.Checked := True;
            if (Checkbox2.Checked) and (TR = 0) then CheckBox1.Checked := False;
          end;
        end else Result := -1;
      end;
    end else Result := -1;
    r1.Free;
    r2.Free;
  end;


var
  I, T : Integer;
  S : String;
begin
  if BottomBufLen < 600 then BottomBufLen := BottomBufLen + 1;
  for I := BottomBufLen downto 2 do
  begin
    BottomBuf[I] := BottomBuf[I-1];
  end;
  BottomBuf[1] := StrToInt(Edit1.Text);
  if TopBufLen < 600 then TopBufLen := TopBufLen + 1;
  for I := TopBufLen downto 2 do
  begin
    TopBuf[I] := TopBuf[I-1];
  end;
  TopBuf[1] := StrToInt(Edit2.Text);

  DrawGraph;
  DrawPane;

  for I := 1 to StringGrid1.RowCount-1 do
  begin
    T := ParseExpression(StringGrid1.Cells[0, I]);
    if T <> 1 then
      StringGrid1.Cells[2, I] := IntToStr(T)
    else
      StringGrid1.Cells[2, I] := IntToStr(ParseAction(StringGrid1.Cells[1, I]));
  end;
  StringGrid1.Refresh;

  S := 'SET' + Edit3.Text + '*' + Edit4.Text + '*';
  if ComPort1.Connected then
  begin
    ComPort1.WriteStr(S);
    ListBox1.Items.Insert(0, 'Sent: ' + S);
  end;
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  Bitmap1.Free;
  Bitmap2.Free;
end;


procedure TForm1.SpeedButton1Click(Sender: TObject);
begin
  StringGrid1.RowCount := StringGrid1.RowCount + 1;
end;

procedure TForm1.SpeedButton2Click(Sender: TObject);
begin
  if StringGrid1.RowCount > 2 then
    StringGrid1.RowCount := StringGrid1.RowCount - 1;
end;

procedure TForm1.StringGrid1DrawCell(Sender: TObject; ACol, ARow: Integer;
  Rect: TRect; State: TGridDrawState);
begin
   if StringGrid1.Cells[2, ARow] = '1' then
   begin
     StringGrid1.Canvas.Brush.Color := $0099FF99;
     StringGrid1.Canvas.FillRect(Rect);
     StringGrid1.Canvas.TextOut(Rect.Left+2, Rect.Top+2, StringGrid1.Cells[ACol, ARow]);
   end;
   if StringGrid1.Cells[2, ARow] = '-1' then
   begin
     StringGrid1.Canvas.Brush.Color := $009999FF;
     StringGrid1.Canvas.FillRect(Rect);
     StringGrid1.Canvas.TextOut(Rect.Left+2, Rect.Top+2, StringGrid1.Cells[ACol, ARow]);
   end;
end;

procedure TForm1.SpeedButton5Click(Sender: TObject);
var
  Ini : TIniFile;
  SaveDialog : TSaveDialog;
  I : Integer;
begin
  SaveDialog := TSaveDialog.Create(self);
  SaveDialog.InitialDir := GetCurrentDir;
  SaveDialog.Filter := 'INI file|*.ini';
  SaveDialog.DefaultExt := 'ini';
  SaveDialog.FilterIndex := 1;
  if SaveDialog.Execute
  then
  begin
    Ini:=TIniFile.Create(SaveDialog.FileName);
    Ini.WriteString('Header','Lines',IntToStr(StringGrid1.RowCount-1));
    for I := 1 to StringGrid1.RowCount-1 do
    begin
      Ini.WriteString('Condition',IntToStr(I),StringGrid1.Cells[0, I]);
      Ini.WriteString('Action',IntToStr(I),StringGrid1.Cells[1, I]);
    end;
    Ini.Free;
  end;
  SaveDialog.Free;
end;

procedure TForm1.SpeedButton3Click(Sender: TObject);
var
  Ini : TIniFile;
  OpenDialog : TOpenDialog;
  I, Lines : Integer;
begin
  OpenDialog := TOpenDialog.Create(self);
  OpenDialog.InitialDir := Edit1.Text;
  OpenDialog.Options := [ofFileMustExist];
  OpenDialog.Filter :='INI file(*.ini)|*.ini|All files|*.*';
  OpenDialog.FilterIndex := 1;
  if (not OpenDialog.Execute) then Exit;
  Ini:=TIniFile.Create(OpenDialog.FileName);
  Lines := StrToInt(Ini.ReadString('Header','Lines','0'));
  StringGrid1.RowCount := Lines + 1;
  StringGrid1.FixedRows := 1;
  for I := 1 to Lines do
  begin
    StringGrid1.Cells[0, I] := Ini.ReadString('Condition', IntToStr(I), '');
    StringGrid1.Cells[1, I] := Ini.ReadString('Action', IntToStr(I), '');
  end;
  OpenDialog.Free;
  Ini.Free;
end;

procedure TForm1.ComPort1RxChar(Sender: TObject; Count: Integer);
var
  Str : String;
begin
  ComPort1.ReadStr(Str, Count);
  if Copy(Str, 1, 2) = 'OK' then T := '';
  T := T + Str;
  if Copy(T, Length(T)-1 ,2) = #13#10 then
  begin
    ListBox1.Items.Insert(0, 'Received: ' + T);
    TopHeaterState := StrToInt(Copy(T, 3, 3));
    BottomHeaterState := StrToInt(Copy(T, 6, 3));
    Edit2.Text:=IntToStr(StrToInt(Copy(T, 9, 3)));
    Edit1.Text:=IntToStr(StrToInt(Copy(T, 12, 3)));
    T := '';
  end;
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  ComPort1.Port := ComboBox1.Text;
  try
    ComPort1.Connected := True
  except
    Listbox1.Items.Insert(0, 'Failed to connect to ' + ComPort1.Port);
  end;
end;

end.
