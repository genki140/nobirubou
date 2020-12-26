VERSION 5.00
Begin VB.Form Form1 
   BackColor       =   &H00FFFFFF&
   BorderStyle     =   1  '固定(実線)
   Caption         =   "ステージエディタ"
   ClientHeight    =   9000
   ClientLeft      =   45
   ClientTop       =   735
   ClientWidth     =   12000
   FillStyle       =   0  '塗りつぶし
   BeginProperty Font 
      Name            =   "ＭＳ Ｐゴシック"
      Size            =   14.25
      Charset         =   128
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   -600
   ScaleLeft       =   -400
   ScaleMode       =   0  'ﾕｰｻﾞｰ
   ScaleTop        =   300
   ScaleWidth      =   800
   StartUpPosition =   2  '画面の中央
   Begin VB.Menu f 
      Caption         =   "ファイル"
      Begin VB.Menu sinki 
         Caption         =   "新規作成"
      End
      Begin VB.Menu opan 
         Caption         =   "開く"
      End
      Begin VB.Menu uwagaki 
         Caption         =   "上書き保存"
         Enabled         =   0   'False
      End
      Begin VB.Menu save 
         Caption         =   "別名で保存"
      End
   End
   Begin VB.Menu p 
      Caption         =   "ペン"
      Begin VB.Menu ゴールブロック 
         Caption         =   "ゴールブロック"
      End
      Begin VB.Menu 普通ブロック 
         Caption         =   "普通ブロック"
      End
      Begin VB.Menu 上に跳ぶ 
         Caption         =   "上に跳ぶ"
      End
      Begin VB.Menu 右に飛ぶ 
         Caption         =   "右に飛ぶ"
      End
      Begin VB.Menu 下に飛ぶ 
         Caption         =   "下に飛ぶ"
      End
      Begin VB.Menu 左に飛ぶ 
         Caption         =   "左に飛ぶ"
      End
      Begin VB.Menu ばね 
         Caption         =   "ばね"
      End
      Begin VB.Menu とげ 
         Caption         =   "とげ"
      End
      Begin VB.Menu あみ 
         Caption         =   "あみ"
      End
      Begin VB.Menu 手が当たる四角 
         Caption         =   "手が当たる四角"
      End
      Begin VB.Menu 体も当たる四角 
         Caption         =   "体も当たる四角"
      End
      Begin VB.Menu 腕が刺さる 
         Caption         =   "腕が刺さる"
      End
      Begin VB.Menu 腕以外刺さる 
         Caption         =   "腕以外刺さる"
      End
      Begin VB.Menu とげ網 
         Caption         =   "とげ網"
      End
      Begin VB.Menu スイッチ1 
         Caption         =   "スイッチ1"
      End
      Begin VB.Menu スイッチブロック1 
         Caption         =   "スイッチブロック1"
      End
      Begin VB.Menu スイッチ2 
         Caption         =   "スイッチ2"
      End
      Begin VB.Menu スイッチブロック2 
         Caption         =   "スイッチブロック2"
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim CameraX As Long
Dim CameraY As Long

Const max = 10000

Dim BokutekiX As Long
Dim BokutekiY As Long
Dim BlockK(max) As Long
Dim BlockX(max) As Long
Dim BlockY(max) As Long

Const W = 20

Dim Pen As Integer

Dim mouse As Boolean

Dim stage As Integer




Private Sub Form_Initialize()
Render (True)
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
If KeyCode = 37 Then
CameraX = CameraX - 1
ElseIf KeyCode = 38 Then
CameraY = CameraY + 1
ElseIf KeyCode = 39 Then
CameraX = CameraX + 1
ElseIf KeyCode = 40 Then
CameraY = CameraY - 1
End If
Render (True)
End Sub

Private Sub Form_Load()
Pen = 2
stage = 0
End Sub

Private Sub Form_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
Dim i As Long

If Button = 1 Then


If Pen = 1 Then
For B = 0 To max - 1
If BlockK(B) = Pen Then
BlockK(B) = 0
End If
Next B
End If

'同じところにブロックがないか確認
For B = 0 To max - 1
If BlockK(B) <> 0 Then
i = X / W + CameraX
If BlockX(B) = i Then
i = Y / W + CameraY
If BlockY(B) = i Then
BlockK(B) = 0
B = max
End If
End If
End If
Next B

For a = 0 To max - 1
'If (Pen <> 1 And Pen <> 5 And BlockK(a) = 0) Or ((pan = 1 Or Pen = 5) And BlockK(a) = Pen) Then

If BlockK(a) = 0 Then
BlockK(a) = Pen
BlockX(a) = X / W + CameraX
BlockY(a) = Y / W + CameraY
a = max
End If

Next a

Else

For a = 0 To max - 1
If BlockK(a) <> 0 Then
i = X / W + CameraX
If BlockX(a) = i Then
i = Y / W + CameraY
If BlockY(a) = i Then
BlockK(a) = 0
End If
End If
End If
Next a

End If
mouse = True
If Button = 1 And Pen <> 1 Then
Render (False)
Else
Render (True)
End If
End Sub

Private Sub Form_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
If mouse = True Then
Form_MouseDown Button, Shift, X, Y
End If
End Sub

Private Sub Form_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
mouse = False
End Sub

Private Sub Form_Paint()
Render (True)
End Sub

Private Sub Form_Resize()
Render (True)
End Sub

Private Sub opan_Click()
If MsgBox("ステージを開くとデータは消えます、よろしいですか？", vbYesNo) = vbYes Then
s = InputBox("開くステージ番号を入力してください(1～1000)")




For i = 0 To max - 1
BlockK(i) = 0
Next i






On Error GoTo ero

s = Int(s)
If s < 1 Or s > 1000 Then
MsgBox "ステージ番号は1～1000までです"
Render (True)
Exit Sub
End If


Dim fairuloop As Long
Dim 文章 As Variant
Dim intFileNo As Integer

fairuloop = 0
intFileNo = FreeFile
Open App.Path & "\Data\Stage\Edit" & s For Input As intFileNo
Do Until EOF(intFileNo)
Line Input #intFileNo, 文章

If fairuloop Mod 3 = 0 Then
BlockK(fairuloop \ 3) = 文章
If BlockK(fairuloop \ 3) = 1000000 Then
BlockK(fairuloop \ 3) = 0
End If
ElseIf fairuloop Mod 3 = 1 Then
BlockX(fairuloop \ 3) = 文章
ElseIf fairuloop Mod 3 = 2 Then
BlockY(fairuloop \ 3) = 文章
End If

fairuloop = fairuloop + 1
Loop
Close intFileNo

uwagaki.Enabled = True
stage = s

End If

Render (True)

Exit Sub
ero:
MsgBox "エラーが発生しました"
Render (True)
End Sub

Private Sub save_Click()
'If MsgBox("同じディレクトリの、Stage.txt に保存します。", vbYesNo) = vbYes Then
s = InputBox("保存するステージ番号を入力してください(1～1000)")


On Error GoTo ero

s = Int(s)
If s < 1 Or s > 1000 Then
MsgBox "ステージ番号は1～1000までです"
Render (True)
Exit Sub
End If


If MsgBox("上書き保存してしまうと以前のデータは消えてしまいます、よろしいですか？", vbYesNo) = vbYes Then



Open App.Path & "\Data\Stage\Edit" & s For Output As #1

For a = 0 To max - 1
If BlockK(a) <> 0 Then
Write #1, BlockK(a)
Write #1, BlockX(a)
Write #1, BlockY(a)
End If
Next a

Write #1, 1000000

Close #1

stage = s
uwagaki.Enabled = True

End If

Render (True)

Exit Sub
ero:
MsgBox "エラーが発生しました"
Render (True)

'End If
End Sub

Private Sub Render(clear As Boolean)
Form1.Caption = CameraX & "," & CameraY

If clear = True Then
Form1.Cls
End If


Form1.ForeColor = RGB(250, 0, 250)
Form1.FillColor = RGB(250, 0, 250)
Form1.Line (0 * W - W / 2 - CameraX * W, 0 * W - W / 2 - CameraY * W)-(0 * W + W / 2 - CameraX * W, 1 * W + W / 2 - CameraY * W), , B


For a = 0 To max - 1
xx = BlockX(a) * W - CameraX * W
yy = BlockY(a) * W - CameraY * W
If BlockK(a) <> 0 And xx < 420 And xx > -420 And yy < 320 And yy > -320 Then

If BlockK(a) = 1 Then
Form1.ForeColor = RGB(0, 0, 250)
Form1.FillColor = RGB(0, 0, 250)
c = "完"
ElseIf BlockK(a) = 2 Then
Form1.ForeColor = RGB(150, 150, 150)
Form1.FillColor = RGB(150, 150, 150)
c = "普"
ElseIf BlockK(a) = 3 Then
Form1.ForeColor = RGB(100, 100, 250)
Form1.FillColor = RGB(100, 100, 250)
c = "上"
ElseIf BlockK(a) = 4 Then
Form1.ForeColor = RGB(100, 100, 250)
Form1.FillColor = RGB(100, 100, 250)
c = "右"
ElseIf BlockK(a) = 5 Then
Form1.ForeColor = RGB(100, 100, 250)
Form1.FillColor = RGB(100, 100, 250)
c = "下"
ElseIf BlockK(a) = 6 Then
Form1.ForeColor = RGB(100, 100, 250)
Form1.FillColor = RGB(100, 100, 250)
c = "左"

ElseIf BlockK(a) = 7 Then
Form1.ForeColor = RGB(100, 100, 250)
Form1.FillColor = RGB(100, 100, 250)
c = "跳"

ElseIf BlockK(a) = 8 Then
Form1.ForeColor = RGB(100, 0, 0)
Form1.FillColor = RGB(100, 0, 0)
c = "全"

ElseIf BlockK(a) = 9 Then
Form1.ForeColor = RGB(150, 150, 150)
Form1.FillColor = RGB(150, 150, 150)
c = "網"

ElseIf BlockK(a) = 10 Then
Form1.ForeColor = RGB(100, 200, 200)
Form1.FillColor = RGB(100, 200, 200)
c = "手"

ElseIf BlockK(a) = 11 Then
Form1.ForeColor = RGB(100, 200, 200)
Form1.FillColor = RGB(100, 200, 200)
c = "体"

ElseIf BlockK(a) = 12 Then
Form1.ForeColor = RGB(100, 0, 0)
Form1.FillColor = RGB(100, 0, 0)
c = "腕"

ElseIf BlockK(a) = 13 Then
Form1.ForeColor = RGB(100, 0, 0)
Form1.FillColor = RGB(100, 0, 0)
c = "手"

ElseIf BlockK(a) = 14 Then
Form1.ForeColor = RGB(150, 150, 150)
Form1.FillColor = RGB(150, 150, 150)
c = "刺"

ElseIf BlockK(a) = 15 Then
Form1.ForeColor = RGB(0, 250, 0)
Form1.FillColor = RGB(0, 250, 0)
c = "S1"
ElseIf BlockK(a) = 16 Then
Form1.ForeColor = RGB(0, 250, 0)
Form1.FillColor = RGB(0, 250, 0)
c = "B1"

ElseIf BlockK(a) = 17 Then
Form1.ForeColor = RGB(0, 250, 0)
Form1.FillColor = RGB(0, 250, 0)
c = "S2"
ElseIf BlockK(a) = 18 Then
Form1.ForeColor = RGB(0, 250, 0)
Form1.FillColor = RGB(0, 250, 0)
c = "B2"


End If

Form1.Line (BlockX(a) * W - W / 2 - CameraX * W, BlockY(a) * W - W / 2 - CameraY * W)-(BlockX(a) * W + W / 2 - CameraX * W, BlockY(a) * W + W / 2 - CameraY * W), , B
Form1.CurrentX = BlockX(a) * W - W / 2 - CameraX * W
Form1.ForeColor = RGB(0, 0, 0)
Form1.CurrentY = BlockY(a) * W + W / 2 - CameraY * W
Form1.Print c


End If
Next a


End Sub

Private Sub sinki_Click()
For i = 0 To max - 1
BlockK(i) = 0
Next i
CameraX = 0
CameraY = 0
Render (True)
stage = 0
End Sub

Private Sub uwagaki_Click()


On Error GoTo ero

If MsgBox("上書き保存してしまうと以前のデータは消えてしまいます、よろしいですか？", vbYesNo) = vbYes Then



Open App.Path & "\Data\Stage\Edit" & stage For Output As #1

For a = 0 To max - 1
If BlockK(a) <> 0 Then
Write #1, BlockK(a)
Write #1, BlockX(a)
Write #1, BlockY(a)
End If
Next a

Write #1, 1000000

Close #1


End If

Render (True)

Exit Sub
ero:
MsgBox "エラーが発生しました"
Render (True)


End Sub

Private Sub あみ_Click()
Pen = 9
End Sub

Private Sub ゴールブロック_Click()
Pen = 1
End Sub

Private Sub スイッチ1_Click()
Pen = 15
End Sub

Private Sub スイッチ2_Click()
Pen = 17
End Sub

Private Sub スイッチブロック1_Click()
Pen = 16
End Sub

Private Sub スイッチブロック2_Click()
Pen = 18
End Sub

Private Sub とげ_Click()
Pen = 8
End Sub

Private Sub とげ網_Click()
Pen = 14
End Sub

Private Sub ばね_Click()
Pen = 7
End Sub

Private Sub 右に飛ぶ_Click()
Pen = 4
End Sub

Private Sub 下に飛ぶ_Click()
Pen = 5
End Sub

Private Sub 左に飛ぶ_Click()
Pen = 6
End Sub

Private Sub 手が当たる四角_Click()
Pen = 10
End Sub

Private Sub 上に跳ぶ_Click()
Pen = 3
End Sub

Private Sub 体も当たる四角_Click()
Pen = 11
End Sub

Private Sub 普通ブロック_Click()
Pen = 2
End Sub

Private Sub 腕が刺さる_Click()
Pen = 12
End Sub

Private Sub 腕以外刺さる_Click()
Pen = 13
End Sub
