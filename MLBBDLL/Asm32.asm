.model flat, c
.data
	attackStatus dword 0059C664h  ; ���ʱ˭�Ĺ����غ�
	SprintfCall dword 00533574h  ; ��Ϸ�Դ�Sprintf()����,����ƴ��
	Socket dword 0D21AE8h		; 5330

.code
	; void playerAttack(index);  ��ҹ�������(�������)
ASM_playerAttack proc
	push ebp
	mov ebp, esp
	mov eax, dword ptr ss:[esp+8h]
	sub esp,20h
	push eax
	lea edx,dword ptr ss:[esp+14h]
	push 054CF10h
	push edx
	mov eax, 0537D15h
	call eax
	mov ecx,dword ptr ds:[0D27E58h]
	lea eax,dword ptr ss:[esp+1Ch]
	push eax
	push ecx
	mov eax, 050AC80h
	call eax
	add esp,14h
	mov eax,dword ptr ds:[05A28ACh]
	mov dword ptr ds:[05A28F4h],2h
	inc eax
	mov dword ptr ds:[05A28ACh],eax
	mov eax, 0491140h
	call eax
	mov eax, 0492930h
	call eax
	add esp,20h
	mov esp, ebp
	pop ebp
	ret
ASM_playerAttack endp

	; void petAttack(Index, Num);  ���﹥������(�������, ����)
ASM_petAttack proc
	push ebp
	mov ebp, esp
	mov eax, dword ptr ss:[esp+8h]
	mov edx, dword ptr ss:[esp+0Ch]
	sub esp, 20h
	lea ecx,dword ptr ss:[esp+0Ch]
	push eax
	push edx
	push 054CED0h
	push ecx
	mov eax, 0537D15h
	call eax
	mov eax,dword ptr ds:[0D27E58h]
	lea ecx,dword ptr ss:[esp+1Ch]
	push ecx
	push eax
	mov eax, 050AC80h
	call eax
	add esp,18h
	mov dword ptr ds:[05A28F4h],5h
	mov eax,dword ptr ds:[05A2874h]
	inc eax
	mov dword ptr ds:[05A2874h],eax
	mov eax, 0491140h
	call eax
	mov eax, 0492930h
	call eax
	add esp, 20h
	mov esp, ebp
	pop ebp
	ret
ASM_petAttack endp

; void CallHome(); �س�
ASM_CallHome proc
	mov ecx,dword ptr ds:[0D27E58h]
	push ecx
	mov eax, 050CFB0h  ; 0050B4D0h +17B8
	call eax
	add esp,4h
	ret
ASM_CallHome endp


;VOID NPCInteract(X, Y);   ���͵㴫��/NPC���� (X����, Y����) ���Դ�NPC�Ի���
ASM_NPCInteract proc	
	mov ecx, dword ptr ss:[esp+4h]
	mov eax, dword ptr ss:[esp+8h]
	mov dword ptr ds:[0C17320h],eax 
	mov dword ptr ds:[0C1731Ch],ecx
	push 1000h
	push 8h
	push eax
	push ecx
	xor edi,edi
	mov eax, 04415C0h
	call eax
	add esp, 10h
	mov eax, 04660B0h
	call eax
	ret
ASM_NPCInteract endp

; int GetRoleCoorY()   ��ȡ���ﵱǰY����
ASM_GetRoleCoorY proc
	mov ecx, 097B458h
	mov eax, 0401EA0h
	call eax
	ret
ASM_GetRoleCoorY endp

; int GetRoleCoorX() ��ȡ���ﵱǰX����
ASM_GetRoleCoorX proc
	mov ecx, 097B440h
	mov eax, 0401EA0h
	call eax
	ret
ASM_GetRoleCoorX endp


; void DoctorReco(int i); ��ʿ����ҽ����Ѫ�ĶԻ������(ѡ���±�1��ʼ)  
ASM_DoctorReco  proc
	mov eax,dword ptr ss:[esp+4h]
	;MOV ecx, dword ptr ss:[esp+8h]
	;mov dword ptr ds:[0CB3694h], ecx
	sub esp,30h
	;shl eax, 1
	;sub eax, 1h
	lea edx,dword ptr ss:[esp+8h]
	push eax
	push 054C144h
	push edx
	mov eax, 0537D15h
	call eax
	lea eax,dword ptr ss:[esp+34h]
	push 0FFh
	lea ecx,dword ptr ss:[esp+18h]
	push eax
	push ecx
	mov eax, 0527130h
	call eax
	mov eax,dword ptr ds:[0C3DF28h]
	mov ecx,dword ptr ds:[0C3DF24h]
	add esp,18h
	lea edx,dword ptr ss:[esp+28h]
	push edx
	push 0
	push eax
	push ecx
	mov ecx, 097B458h
	mov eax, 0401EA0h
	call eax
	push eax
	mov ecx, 097B440h
	mov eax, 0401EA0h
	call eax
	mov edx,dword ptr ds:[0D27E58h]
	push eax
	push edx
	mov eax, 050C8C0h
	call eax
	mov dword ptr ds:[057A6F0h], -1
	add esp,1Ch
	add esp,30h
	ret
ASM_DoctorReco  endp

	; �������̵� ����(���̵�ڼ��� ��/���� ��1��ʼ��
ASM_OpenShop proc
	mov eax, dword ptr ss:[esp+4h]
	sub esp,20h
	push eax
	lea ecx,dword ptr ss:[esp+4h]
	push 054C144h
	push ecx
	mov eax, 0537D15h
	call eax
	lea edx,dword ptr ss:[esp+1Ch]
	push 0Fh
	lea eax,dword ptr ss:[esp+10h]
	push edx
	push eax
	mov eax, 0527130h
	call eax
	mov edx,dword ptr ds:[0CBAA04h]
	mov eax,dword ptr ds:[0C4ED78h]
	add esp,18h
	lea ecx,dword ptr ss:[esp+10h]
	push ecx
	push 0
	push edx
	push eax
	mov ecx, 097B458h
	mov eax, 0401EA0h
	call eax
	push eax
	mov ecx, 097B440h
	mov eax, 0401EA0h
	call eax
	mov ecx,dword ptr ds:[0D27E58h]
	push eax
	push ecx
	mov eax, 050C8C0h
	call eax
	add esp,01Ch
	add esp,020h
	ret
ASM_OpenShop endp

; void ReadyOK(__int32 Index); // NPC�Ի������(Index) ȷ��4/ȡ��8/�ر�1
ASM_ReadyOK proc
	mov ebx, dword ptr ss:[esp+4]
	sub esp, 30h
	mov edx,dword ptr ds:[0C3DF28h]
	mov eax,dword ptr ds:[0C3DF24h]
	lea ecx,dword ptr ss:[esp+8h]
	push ecx
	push ebx
	push edx
	push eax
	mov ecx, 097B458h
	mov eax, 0401EA0h
	call eax
	push eax
	mov ecx, 097B440h
	mov eax, 0401EA0h
	call eax
	mov ecx,dword ptr ds:[0D27E58h]
	push eax
	push ecx
	mov eax, 050C8C0h
	call eax
	push 0f0h
	push 0140h
	push 04fh
	mov dword ptr ds:[057A6F0h], -1
	mov eax, 05262B0h
	call eax
	push 0f0h
	push 140h
	push 039h
	mov eax, 005262B0h
	call eax
	add esp,034h
	add esp,030h
	ret
ASM_ReadyOK endp

;void UseSkill(__int32 Index, __int32 Num); // ��ս��ʹ�ü���(������1��ʼ)
ASM_UseSkill  proc
	mov eax, dword ptr ss:[esp+4]
	mov ebx, dword ptr ss:[esp+8]
	push ebx
	push eax
	mov eax, 047FCB0h
	call eax
	add esp,8h
	ret
ASM_UseSkill  endp

; �ָ���������ʹ��(�����ڼ������±�, ���ܵȼ��±�)�� ��1��ʼ
ASM_useSkillRecov proc
	mov ebx, dword ptr ss:[esp+4]
	mov eax,dword ptr ds:[0CBB5F8h]
	mov ecx,dword ptr ds:[0D27E58h]
	push eax
	push ecx
	mov dword ptr ds:[0CBBEC4h],eax
	mov dword ptr ds:[0C2D65Ch],ebx
	mov eax, 050BED0h
	call eax
	add esp,8
	ret
ASM_useSkillRecov endp

; void useRecovItemOne(Index); ʹ�ûָ���������ѡ�����(�Լ����߶���),�±��1��ʼ
ASM_useRecovItemOne proc
	mov ecx, dword ptr ss:[esp+4]
	mov eax, dword ptr ds:[0C2D658h]
	mov edx, dword ptr ds:[0CBCCD0h]
	mov edx, dword ptr ds:[edx+ecx*4]
	push edx
	push ecx
	call eax
	add esp, 8
	mov byte ptr ds:[0C2CBA0h],0
	ret
ASM_useRecovItemOne endp


; void useRecovItemTwo(Index); ʹ�ûָ���������ѡ��Ŀ��(�Լ����߳���),�±��1��ʼ
ASM_useRecovItemTwo proc
	mov ebx, dword ptr ss:[esp+4]
	mov eax,dword ptr ds:[0C8136Ch]
	mov edx,dword ptr ds:[0C3CF68h]
	lea ecx,dword ptr ds:[ebx*8]
	sub ecx,ebx
	mov ecx,dword ptr ds:[edx+ecx*4]
	push ecx
	push ebx
	call eax
	add esp, 8
	mov byte ptr ds:[0C27508h],0
	ret
ASM_useRecovItemTwo endp

ASM_GetAddreByte proc
	mov ecx, dword ptr ss:[esp+4]
	xor eax, eax
	mov al, byte ptr ds:[ecx]
	ret
ASM_GetAddreByte endp

ASM_GetAddreWord proc
	mov ecx, dword ptr ss:[esp+4]
	xor eax, eax
	mov ax, word ptr ds:[ecx]
	ret
ASM_GetAddreWord endp

ASM_GetAddreDword proc
	mov ecx, dword ptr ss:[esp+4]
	xor eax, eax
	mov eax, dword ptr ds:[ecx]
	ret
ASM_GetAddreDword endp

; RoleWalk��__int32 Go, int sub��;  ��·(���� 0-7������)
ASM_RoleWalk  proc
	mov ax, word ptr ds:[esp+4]
	mov cx, word ptr ds:[esp+8]
	mov word ptr ds:[0C0ED98h],cx
	mov ebx,0A7DB90h
	mov word ptr ds:[ebx],ax
	mov word ptr ds:[ebx+2],ax
	mov word ptr ds:[ebx+4],ax
	mov word ptr ds:[ebx+6],ax
	mov word ptr ds:[ebx+8],ax
	mov word ptr ds:[ebx+0ah],ax
	mov word ptr ds:[ebx+0ch],ax

	ret
ASM_RoleWalk  endp

; SellGoods(Index)  ����������Ʒ(������Ʒ(��Ʒ�ڱ������±���ַ���))
ASM_SellShop proc
	mov edx, dword ptr ds:[esp+4]
	sub esp, 10h
	mov eax,dword ptr ds:[0CBAA04h]
	mov ecx,dword ptr ds:[0C4ED78h]
	push edx
	push 0
	push eax
	push ecx
	mov ecx, 097B458h
	mov eax, 0401EA0h
	call eax
	push eax
	mov ecx, 097B440h
	mov eax, 0401EA0h
	call eax
	mov edx,dword ptr ds:[0D27E58h]
	push eax
	push edx
	mov eax, 050C8C0h
	call eax
	add esp,01Ch
	add esp, 10h
	ret
ASM_SellShop endp

; ������
ASM_DeleteGoods proc
	mov eax, dword ptr ss:[esp+4]
	add eax, 7
	push eax
	push 0
	mov ecx, 097B458h
	mov eax, 0401EA0h
	call eax
	push eax
	mov ecx, 097B440h
	mov eax, 0401EA0h
	call eax
	mov edx,dword ptr ds:[0D27E58h]
	push eax
	push edx
	mov eax, 050AFB0h
	call eax
	add esp,014h
	ret
ASM_DeleteGoods endp

; ��ǰ���������ʹ��(������1��ʼ)
ASM_WorkOver proc
	mov eax, dword ptr ss:[esp+4]
	mov ecx, dword ptr ss:[esp+8]
	push 058A510h
	push -1
	push ecx
	mov ecx,dword ptr ds:[0D27E58h]
	push eax
	push ecx
	mov eax, 050C190h
	call eax
	add esp,014h
	ret
ASM_WorkOver endp


; �����ظ���رյ�ǰ�ظ�״̬    ------>
;ASM_CloseRecov proc
;	push 0
;	mov eax, 005144D0h
;	call eax
;	add esp, 4
;	mov eax,dword ptr ds:[0C21628h]
;	mov ecx,dword ptr ds:[eax]
;	or ch,80h
;	mov dword ptr ds:[eax],ecx
;	ret
;ASM_CloseRecov endp


; װ����Ʒ(��Ʒ�ڱ�����λ��, ��Ʒװ����λ��)
ASM_WearItem proc
	mov eax, dword ptr ds:[esp+4]
	mov ecx, dword ptr ds:[esp+8]
	push 0ffffffffh
	push ecx
	push eax
	mov eax,dword ptr ds:[0D27E58h]
	push eax
	mov eax, 050B190h
	call eax
	add esp,10h
	ret
ASM_WearItem endp


ASM_RBtnDown proc  ; ---->
	mov eax, dword ptr ss:[esp+4]
	push eax
	push esp
	mov ecx, 097B458h
	mov eax, 0401EA0h
	call eax
	push eax
	mov ecx, 097B440h
	mov eax, 0401EA0h
	call eax
	push eax
	mov eax, 0506BC0h
	call eax
	add esp,010h
	ret
ASM_RBtnDown endp

; ����/��ֹ���
ASM_IsAddArmy proc
	mov eax, 046DA90h
	call eax
	ret
ASM_IsAddArmy endp

; ����/��ֹ����
ASM_IsTrade proc
	mov eax, 046DAF0h
	call eax
	ret
ASM_IsTrade endp

;�������
ASM_AddArmy proc
	mov eax, 046AF80h
	call eax
	ret
ASM_AddArmy endp

; ���ͽ�������
ASM_Trade proc
	mov eax, 046B1B0h
	call eax
	ret
ASM_Trade endp

; ѡ���׶���(0)��  ASM_TradeChoice(0);   �±���㿪ʼ 
ASM_TradeChoice proc
	mov eax, dword ptr ss:[esp+4]
		mov esi, 0Eh
	mov ecx,dword ptr ds:[0C3E9ACh] 
	mov dword ptr ds:[0C50A8Ch], eax
	lea eax, dword ptr ds:[eax+esi-0Eh]
	mov edx, eax
	shl edx,4
	lea edx,dword ptr ds:[edx+eax+0C80ABCh]
	push edx
	push eax
	call ecx
	add esp, 8
	mov eax,dword ptr ds:[0C8122Ch]
	push 0F0h
	push 140h
	push 39h
	or dword ptr ds:[eax],08000h
	mov eax, 05262B0h
	call eax
	add esp,0Ch
	ret
ASM_TradeChoice endp

; ������Ʒ_���(�������, ��������Ʒ���Ӻ�Ҫ���ڽ������ϵĸ���)  ASM_TradeGoods(int Num�� char* Buf);
ASM_TradeGoods proc
	mov edx, dword ptr ss:[esp+4]
	mov ebx, dword ptr ss:[esp+8]
	mov dword ptr ds:[0C91268h], edx
	push ebp
	sub esp, 100h
	mov dword ptr ss:[esp+80h], 0
	lea eax,dword ptr ss:[esp+80h]
	mov dword ptr ss:[esp+50h], 0
	lea ecx, dword ptr ss:[esp+50h]
	push 30h
	push eax
	push ecx
	mov eax, 05388D0h
	call eax
	mov edx,dword ptr ds:[0C91268h]
	lea eax,dword ptr ss:[esp+58h]
	push edx
	mov edx, dword ptr ds:[0D27E58h]
	push eax
	push ebx
	push edx
	mov eax, 050C2E0h
	call eax
	mov dword ptr ds:[0CBAE48h],1
	mov dword ptr ds:[0CF3FD0h],0
	add esp, 1Ch
	add esp, 100h
	pop ebp
	ret
ASM_TradeGoods endp


; ����ȷ��
ASM_TradeOK proc
	mov edx,dword ptr ds:[0D27E58h]
	push ebp
	mov ebp , 1
	push ebp
	push edx
	mov eax, 050C510h
	call eax
	add esp, 8
	mov dword ptr ds:[0CBAE48h],2
	mov dword ptr ds:[0CF4858h],ebp
	mov dword ptr ds:[0CF485Ch],ebp
	pop ebp
	ret
ASM_TradeOK endp

; ��Ϸ�ڴ�ӡ()  ASM_Print(char* )
ASM_Print proc
	mov ecx, dword ptr ss:[esp+4]
	push 3
	push 4
	push ecx
	mov eax, 04475D0h
	call eax
	add esp, 0Ch
	ret
ASM_Print endp


; ��������Ե� �����±�(����0,����1,ǿ��2, �ٶ�3,ħ��4)  ASM_RoleAddAttr(int)
ASM_RoleAddAttr proc
	mov eax, dword ptr ss:[esp+4]
	mov ecx, dword ptr ds:[0D27E58h]
	push eax
	push ecx
	mov eax, 050C650h
	call eax
	add esp, 8
	ret
ASM_RoleAddAttr endp

; ��������Ե�
ASM_AddPetsAttr  proc
	mov eax, dword ptr ss:[esp+4]
	push eax
	mov edx,dword ptr ds:[0C509F4h]
	mov ecx,dword ptr ds:[0D27E58h]
	movsx eax,word ptr ds:[edx*8+0D6D7B2h]
	push eax
	push ecx
	mov eax, 050C6A0h
	call eax
	add esp, 0Ch
	ret
ASM_AddPetsAttr endp

;�ٻ�����
ASM_ComeBady proc
	mov eax, dword ptr ss:[esp+4]
	push 0
	push 0
	push 0
	mov edx,dword ptr ds:[0D27E58h]
	push 0
	push eax
	push edx
	mov eax, 050BD80h
	call eax
	add esp, 18h
	ret
ASM_ComeBady endp
end
