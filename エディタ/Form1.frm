VERSION 5.00
Begin VB.Form Form1 
   BackColor       =   &H00FFFFFF&
   BorderStyle     =   1  '�Œ�(����)
   Caption         =   "�X�e�[�W�G�f�B�^"
   ClientHeight    =   9000
   ClientLeft      =   45
   ClientTop       =   735
   ClientWidth     =   12000
   FillStyle       =   0  '�h��Ԃ�
   BeginProperty Font 
      Name            =   "�l�r �o�S�V�b�N"
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
   ScaleMode       =   0  'հ�ް
   ScaleTop        =   300
   ScaleWidth      =   800
   StartUpPosition =   2  '��ʂ̒���
   Begin VB.Menu f 
      Caption         =   "�t�@�C��"
      Begin VB.Menu sinki 
         Caption         =   "�V�K�쐬"
      End
      Begin VB.Menu opan 
         Caption         =   "�J��"
      End
      Begin VB.Menu uwagaki 
         Caption         =   "�㏑���ۑ�"
         Enabled         =   0   'False
      End
      Begin VB.Menu save 
         Caption         =   "�ʖ��ŕۑ�"
      End
   End
   Begin VB.Menu p 
      Caption         =   "�y��"
      Begin VB.Menu �S�[���u���b�N 
         Caption         =   "�S�[���u���b�N"
      End
      Begin VB.Menu ���ʃu���b�N 
         Caption         =   "���ʃu���b�N"
      End
      Begin VB.Menu ��ɒ��� 
         Caption         =   "��ɒ���"
      End
      Begin VB.Menu �E�ɔ�� 
         Caption         =   "�E�ɔ��"
      End
      Begin VB.Menu ���ɔ�� 
         Caption         =   "���ɔ��"
      End
      Begin VB.Menu ���ɔ�� 
         Caption         =   "���ɔ��"
      End
      Begin VB.Menu �΂� 
         Caption         =   "�΂�"
      End
      Begin VB.Menu �Ƃ� 
         Caption         =   "�Ƃ�"
      End
      Begin VB.Menu ���� 
         Caption         =   "����"
      End
      Begin VB.Menu �肪������l�p 
         Caption         =   "�肪������l�p"
      End
      Begin VB.Menu �̂�������l�p 
         Caption         =   "�̂�������l�p"
      End
      Begin VB.Menu �r���h���� 
         Caption         =   "�r���h����"
      End
      Begin VB.Menu �r�ȊO�h���� 
         Caption         =   "�r�ȊO�h����"
      End
      Begin VB.Menu �Ƃ��� 
         Caption         =   "�Ƃ���"
      End
      Begin VB.Menu �X�C�b�`1 
         Caption         =   "�X�C�b�`1"
      End
      Begin VB.Menu �X�C�b�`�u���b�N1 
         Caption         =   "�X�C�b�`�u���b�N1"
      End
      Begin VB.Menu �X�C�b�`2 
         Caption         =   "�X�C�b�`2"
      End
      Begin VB.Menu �X�C�b�`�u���b�N2 
         Caption         =   "�X�C�b�`�u���b�N2"
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

'�����Ƃ���Ƀu���b�N���Ȃ����m�F
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
If MsgBox("�X�e�[�W���J���ƃf�[�^�͏����܂��A��낵���ł����H", vbYesNo) = vbYes Then
s = InputBox("�J���X�e�[�W�ԍ�����͂��Ă�������(1�`1000)")




For i = 0 To max - 1
BlockK(i) = 0
Next i






On Error GoTo ero

s = Int(s)
If s < 1 Or s > 1000 Then
MsgBox "�X�e�[�W�ԍ���1�`1000�܂łł�"
Render (True)
Exit Sub
End If


Dim fairuloop As Long
Dim ���� As Variant
Dim intFileNo As Integer

fairuloop = 0
intFileNo = FreeFile
Open App.Path & "\Data\Stage\Edit" & s For Input As intFileNo
Do Until EOF(intFileNo)
Line Input #intFileNo, ����

If fairuloop Mod 3 = 0 Then
BlockK(fairuloop \ 3) = ����
If BlockK(fairuloop \ 3) = 1000000 Then
BlockK(fairuloop \ 3) = 0
End If
ElseIf fairuloop Mod 3 = 1 Then
BlockX(fairuloop \ 3) = ����
ElseIf fairuloop Mod 3 = 2 Then
BlockY(fairuloop \ 3) = ����
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
MsgBox "�G���[���������܂���"
Render (True)
End Sub

Private Sub save_Click()
'If MsgBox("�����f�B���N�g���́AStage.txt �ɕۑ����܂��B", vbYesNo) = vbYes Then
s = InputBox("�ۑ�����X�e�[�W�ԍ�����͂��Ă�������(1�`1000)")


On Error GoTo ero

s = Int(s)
If s < 1 Or s > 1000 Then
MsgBox "�X�e�[�W�ԍ���1�`1000�܂łł�"
Render (True)
Exit Sub
End If


If MsgBox("�㏑���ۑ����Ă��܂��ƈȑO�̃f�[�^�͏����Ă��܂��܂��A��낵���ł����H", vbYesNo) = vbYes Then



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
MsgBox "�G���[���������܂���"
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
c = "��"
ElseIf BlockK(a) = 2 Then
Form1.ForeColor = RGB(150, 150, 150)
Form1.FillColor = RGB(150, 150, 150)
c = "��"
ElseIf BlockK(a) = 3 Then
Form1.ForeColor = RGB(100, 100, 250)
Form1.FillColor = RGB(100, 100, 250)
c = "��"
ElseIf BlockK(a) = 4 Then
Form1.ForeColor = RGB(100, 100, 250)
Form1.FillColor = RGB(100, 100, 250)
c = "�E"
ElseIf BlockK(a) = 5 Then
Form1.ForeColor = RGB(100, 100, 250)
Form1.FillColor = RGB(100, 100, 250)
c = "��"
ElseIf BlockK(a) = 6 Then
Form1.ForeColor = RGB(100, 100, 250)
Form1.FillColor = RGB(100, 100, 250)
c = "��"

ElseIf BlockK(a) = 7 Then
Form1.ForeColor = RGB(100, 100, 250)
Form1.FillColor = RGB(100, 100, 250)
c = "��"

ElseIf BlockK(a) = 8 Then
Form1.ForeColor = RGB(100, 0, 0)
Form1.FillColor = RGB(100, 0, 0)
c = "�S"

ElseIf BlockK(a) = 9 Then
Form1.ForeColor = RGB(150, 150, 150)
Form1.FillColor = RGB(150, 150, 150)
c = "��"

ElseIf BlockK(a) = 10 Then
Form1.ForeColor = RGB(100, 200, 200)
Form1.FillColor = RGB(100, 200, 200)
c = "��"

ElseIf BlockK(a) = 11 Then
Form1.ForeColor = RGB(100, 200, 200)
Form1.FillColor = RGB(100, 200, 200)
c = "��"

ElseIf BlockK(a) = 12 Then
Form1.ForeColor = RGB(100, 0, 0)
Form1.FillColor = RGB(100, 0, 0)
c = "�r"

ElseIf BlockK(a) = 13 Then
Form1.ForeColor = RGB(100, 0, 0)
Form1.FillColor = RGB(100, 0, 0)
c = "��"

ElseIf BlockK(a) = 14 Then
Form1.ForeColor = RGB(150, 150, 150)
Form1.FillColor = RGB(150, 150, 150)
c = "�h"

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

If MsgBox("�㏑���ۑ����Ă��܂��ƈȑO�̃f�[�^�͏����Ă��܂��܂��A��낵���ł����H", vbYesNo) = vbYes Then



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
MsgBox "�G���[���������܂���"
Render (True)


End Sub

Private Sub ����_Click()
Pen = 9
End Sub

Private Sub �S�[���u���b�N_Click()
Pen = 1
End Sub

Private Sub �X�C�b�`1_Click()
Pen = 15
End Sub

Private Sub �X�C�b�`2_Click()
Pen = 17
End Sub

Private Sub �X�C�b�`�u���b�N1_Click()
Pen = 16
End Sub

Private Sub �X�C�b�`�u���b�N2_Click()
Pen = 18
End Sub

Private Sub �Ƃ�_Click()
Pen = 8
End Sub

Private Sub �Ƃ���_Click()
Pen = 14
End Sub

Private Sub �΂�_Click()
Pen = 7
End Sub

Private Sub �E�ɔ��_Click()
Pen = 4
End Sub

Private Sub ���ɔ��_Click()
Pen = 5
End Sub

Private Sub ���ɔ��_Click()
Pen = 6
End Sub

Private Sub �肪������l�p_Click()
Pen = 10
End Sub

Private Sub ��ɒ���_Click()
Pen = 3
End Sub

Private Sub �̂�������l�p_Click()
Pen = 11
End Sub

Private Sub ���ʃu���b�N_Click()
Pen = 2
End Sub

Private Sub �r���h����_Click()
Pen = 12
End Sub

Private Sub �r�ȊO�h����_Click()
Pen = 13
End Sub
