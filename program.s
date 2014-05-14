main:
# carrega para o R2
        addiu $2, $zero, 6
# carrega para o R3
        addiu $3, $zero, 6
# pula
	beq $2, $3, teste
# armazena o R4 na posicao 32
	sw  $2, 32($zero)
# carrega para o R2
        nop
        nop
teste:
        addiu $2, $zero, 7


