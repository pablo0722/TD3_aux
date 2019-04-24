;********************************************************
;						PUSH							*
;********************************************************
	// Formato: 
		PUSH valor1, valor2, ..., valorN

	// Se traduce como:
		push valor1
		push valor2
		...
		push valorN

	// Ejemplo: 
		PUSH eax, ebx, 0x1234

	// Se traduce como:
		push eax
		push ebx
		push 0x1234










;********************************************************
;						POP 							*
;********************************************************
	// Formato:
		POP valor1, valor2, ..., valorN

	// Se traduce como:
		pop valorN
		...
		pop valor2
		pop valor1

	// Ejemplo: 
		POP eax, ebx, 0x1234

	// Se traduce como:
		pop 0x1234
		pop ebx
		pop eax










;********************************************************
;						CALL 							*
;********************************************************
	// Formato:
		CALL func, arg1, arg2, ..., argN

	// Se traduce como:
		push argN
		...
		push arg2
		push arg1
		call func
		add esp, 4*N

	// Ejemplo: 
		CALL memcopy, 0xFFFF0000, 0x00000000, 0x10000

	// Se traduce como:
		push 0x10000
		push 0x00000000
		push 0FFFF0000
		call memcopy
		add esp 12










;********************************************************
;						IF_EQ 							*
;********************************************************
	// Formato: 
	IF_EQ valor01, valor2, 	\
		{instruccion1}, 	\
		{instruccion2}, 	\
		{...}, 				\
		{instruccionN}

	// Se traduce como:
	if(valor1 == valor2)
	{
		instruccion1
		instruccion2
		...
		instruccionN
	}

	// Ejemplo: 
	IF_EQ eax, 0x1C, 	\
		{push eax}, 	\
		{mov ebx, eax}, \
		{pop eax}

	// Se traduce como:
	if(eax == 0x1C)
	{
		push eax;
		mov ebx, eax;
		pop eax;
	}











;********************************************************
;						LOOP 							*
;********************************************************
	// Formato: 
	LOOP cant, 			\
		{instruccion1}, \
		{instruccion2}, \
		{...}
		{instruccionN}

	// Se traduce como:
	for(ecx=cant; ecx>0, ecx--)
	{
		instruccion1
		instruccion2
		...
		instruccionN
	}

	// Ejemplo: 
	LOOP 100, 			\
		{push eax}, 	\
		{mov ebx, eax}, \
		{pop eax}

	// Se traduce como:
	for(ecx=100; ecx>0, ecx--)
	{
		push eax;
		mov ebx, eax;
		pop eax;
	}