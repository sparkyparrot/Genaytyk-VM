// GenaytykVM.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"


#define ROL_DW(D,N) ((D<<N)|(D>>(32-N)))
#define ROR_DW(D,N) ((D<<(32-N))|(D>>N))
#define ROL_B(D,N) ((D<<N)|(D>>(8-N)))
#define ROR_B(D,N) ((D<<(8-N))|(D>>N))



TCHAR StringHardcoded[] = "aAb0cBd1eCf2gDh3jEk4lFm5nGp6qHr7sJt8uKv9w";



// Funciones para el serial
int CrackmeCheckeaDosGuionesCambialosPor0s(TCHAR *cadena)
/*
*		Correspondiente a la VIRTUAL_FUNCTION_000001f9
*/
{
	int i = 0;
	int longitud = _tcsclen(cadena);
	int contador = 0;
	for (i = 0; i < longitud; i++)
	{
		if (cadena[i] == '-'){
			cadena[i] = 0;
			contador += 1;
		}
	}
	if (contador == 2)
		return 0;
	else
		return -1;
}

int CrackmeStrlen(TCHAR *cadena)
/*
*	Correspondiente a VIRTUAL_FUNCTION_0000025d
*/
{
	return _tcsclen(cadena);
}

int CrackmeSetCharset(TCHAR *cadena)
/*
*	Correspondiente a VIRTUAL_FUNCTION_0000027d
*/
{
	int i, j;
	int longitudCadena = _tcsclen(cadena);
	int longitudHardCoded = _tcsclen(StringHardcoded);

	for (i = 0; i < longitudCadena; i++)
	{
		for (j = 0; j < longitudHardCoded; j++)
		{
			if (cadena[i] == StringHardcoded[j])
			{
				cadena[i] = j;
			}
		}
	}
	return 0;
}

int CrackmeHasheaCadenaSerial(TCHAR *cadena)
/*
*		Correspondiente a VIRTUAL_FUNCTION_000002af
*/
{
	unsigned int value_aux = 0;
	unsigned int hash = 0;
	TCHAR *principioCadena = cadena;
	int valorMultiplicar = 1;

	cadena += 5;
	principioCadena -= 1;

	while(1)
	{
		value_aux = (BYTE)(*cadena);
		value_aux *= valorMultiplicar;
		hash += value_aux;
		valorMultiplicar *= 0x29;
		cadena -= 1;

		if (cadena == principioCadena)
			break;
	}

	cadena += 1;
	return hash;
}


// Funciones para el nombre
int CrackmeObtenBytesDeNombre(TCHAR *nombre, BYTE *buffer)
/*
*		Correspondiente a VIRTUAL_FUNCTION_000002ff
*/
{
	int i;
	BYTE value = 0;
	unsigned int valueSuma = 0;
	TCHAR *punteroNombre = nombre;

	for (i = 0x5f; i >= 0; i--)
	{
		value ^= (*punteroNombre);
		value ^= 0x75;
		value += (BYTE)(valueSuma & 0xFF);

		valueSuma += 0x3;
		valueSuma ^= 0x45;
		valueSuma = ROL_DW(valueSuma, 0x3);

		punteroNombre += 1;

		buffer[i] = value;

		if ((*punteroNombre) == 0)
			punteroNombre = nombre;
	}

	return 0;
}

unsigned int CrackmeObtenHashDeNombre(BYTE *arrayOfBytesFromNumber)
/*
*		Correspondiente a VIRTUAL_FUNCTION_0000036c
*/
{
	unsigned int a = 0x1a2b3c4d;
	unsigned int c = 0;
	unsigned int contador = 0xff;

	while (1)
	{
		BYTE *puntero1 = arrayOfBytesFromNumber;
		BYTE *puntero2 = arrayOfBytesFromNumber;
		puntero2 += 0x5d;

		while (puntero1 != puntero2)
		{
			c = *((LPUINT)puntero1);
			a ^= c;
			a = ROL_DW(a, 0x7);
			puntero1 += 1;
		}
		contador -= 1;
		if (contador == 0)
			break;
	}

	return a;
}

int CrackmeObtenParValoresDeNombre(BYTE *arrayOfBytesFromNumber,unsigned int *valoresHasheados)
/*
*		Correspondiente a VIRTUAL_FUNCTION_00000417
*/
{
	unsigned int a = 0x13a1dcb4;
	unsigned int b = 0x45e8adc1;
	unsigned int c = 0x0;
	unsigned int contador = 0x28;

	BYTE *puntero1;
	BYTE *puntero2;
	BYTE *puntero3;

	while (1)
	{
		puntero1 = arrayOfBytesFromNumber;
		puntero2 = arrayOfBytesFromNumber;
		puntero2 += 0x30;
		puntero3 = puntero2;

		while (puntero1 != puntero3)
		{
			a ^= *((LPUINT)puntero1);
			a = ROL_DW(a, 0x3);
			a += *((LPUINT)puntero2);
			puntero1 += 4;
			puntero2 += 4;
		}

		a ^= b;

		c = b;

		b = a;
		a = c;

		contador -= 1;
		if (contador == 0)
			break;
	}
	valoresHasheados[0] = a;
	valoresHasheados[1] = b;

	return 0;
}

unsigned int CrackmeSumaValoresBuffer(BYTE *arrayOfBytesFromNumber)
/*
*	Correspondiente a VIRTUAL_FUNCTION_000003f2
*/
{
	unsigned int value = 0;
	BYTE *puntero1 = arrayOfBytesFromNumber;
	BYTE *puntero2 = arrayOfBytesFromNumber;
	puntero2 += 0x5D;
	while (puntero1 != puntero2)
	{
		value += *((LPUINT)puntero1);
		puntero1 += 1;
	}
	return value;
}

// Funciones de cifrado (creo)
void PCODE_403B40(DWORD d0, DWORD d1, PDWORD result)
{
	UCHAR buff[8] = { 0 };

	*(PDWORD)(buff) = d0;
	*(PDWORD)(buff + 4) = d1;

	PDWORD p0 = (PDWORD)buff;
	PDWORD p1 = (PDWORD)(buff + 4);

	for (int i = 0; i<7; ++i)
	{
		buff[i] += buff[i + 1];
		buff[i] -= i;
		*p0 = ROL_DW(*p0, 5);
		*p1 = ROR_DW(*p1, 5);
	}

	LONG A = 0;
	LONG B = 0x45;

	for (int i = 0; i<4; ++i)
	{
		A = A + buff[i] + 1;
		B *= B;
		A *= B;
	}

	LONG C = 0;
	LONG D = 0x35;

	for (int i = 4; i<8; ++i)
	{
		C = C - buff[i] + 1;
		D *= D;
		C *= D;
	}

	*result = A * C;
}

void decrypt(DWORD k0, DWORD k1, DWORD c0, DWORD c1, PDWORD p0, PDWORD p1, BOOL first_call)
{
	UCHAR key[8] = { 0 };
	UCHAR dwords[8] = { 0 };

	*(PDWORD)(key) = k0;
	*(PDWORD)(key + 4) = k1;

	*(PDWORD)(dwords) = c0;
	*(PDWORD)(dwords + 4) = c1;

	PDWORD pk0 = (PDWORD)key;
	PDWORD pk1 = (PDWORD)(key + 4);
	PDWORD pc0 = (PDWORD)dwords;
	PDWORD pc1 = (PDWORD)(dwords + 4);

	DWORD from_pair;
	PCODE_403B40(*pk0, *pk1, &from_pair);

	for (int k = 0; k<((first_call) ? 10 : 15); ++k)
	{
		// this is inlined pcode from 403916:
		//
		for (int x = 0; x<100; ++x)
		{

			*pc1 = ROL_DW(*pc1, 7);
*pc0 = ROR_DW(*pc0, 7);

for (int i = 7; i >= 0; --i)
{
	dwords[i] = ROR_B(dwords[i], 3);
	dwords[7 - i] ^= dwords[i];
}

int lookupa[33] = { 'X', 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4 };
int lookupb[33] = { 'X', 7, 6, 5, 4, 3, 2, 1, 0, 7, 6, 5, 4, 3, 2, 1, 0, 7, 6, 5, 4, 3, 2, 1, 0, 7, 6, 5, 4, 3, 2, 1, 0 };

for (int i = 1; i <= 32; ++i)
{
	int a = lookupa[i];
	int b = lookupb[i];

	if (from_pair & (1 << i))
		dwords[a] ^= key[b];
	else
	{
		dwords[b] ^= b;
		dwords[b] = ~dwords[b];
	}
}

for (int i = 0; i < 16; ++i)
{
	*pc1 -= *pk0;
	*pc0 -= *pk1;

	*pc1 ^= *pk1;
	*pc0 ^= *pk0;

	*pc1 = ROL_DW(*pc1, 3);
	*pc0 = ROR_DW(*pc0, 3);
}

for (int i = 6; i >= 0; --i)
{
	dwords[i] -= 0x12;
	dwords[i] += key[i + 1];
	dwords[i] ^= key[i];
}
		}
	}

	*p0 = *pc0;
	*p1 = *pc1;
}



void base41_encode(DWORD d, PCHAR p)
{
	memset(p, 'a', 6); p[7] = 0; // leading 0 in character_set

	for (int i = 5; i >= 0 && d; --i)
	{
		p[i] = StringHardcoded[d % 41];
		d /= 41;
	}
}

void INVERSE_PCODE_40355B(PDWORD p0, PDWORD p1, PDWORD p2)
{

	*p2 += *p0;
	*p2 -= *p1;
	*p0 ^= *p1;
	*p2 = ROL_DW(*p2, 11);
	*p1 = ROL_DW(*p1, 9);
	*p0 = ROR_DW(*p0, 7);
}


int main(int argc, LPTSTR *argv)
{
	// variables de nombre


	TCHAR myNombre[37] = { 0 };
	BYTE bufferNombre[0x60] = { 0 };
	unsigned int hashNombre = 0;
	unsigned int parValoresNombre[2] = { 0 };
	unsigned int sumaValoresBuffer = 0;

	// variables de serial
	TCHAR mySerial[113] = "123456-123456-123456";
	TCHAR *punteroSerial = NULL;
	unsigned int serialHash[3] = { 0 };


	if (argc != 2)
	{
		fprintf(stderr, "[-] No dado un nombre usando por defecto: FARE9");
		_tcscpy(myNombre, "FARE9");
	}
	else
	{
		if (_tcsclen(argv[1]) > 36)
		{
			fprintf(stderr, "[-] Nombre especificado demasiado largo, usando por defecto: FARE9");
			_tcscpy(myNombre, "FARE9");
		}
		else
		{
			_tcscpy(myNombre, argv[1]);
		}
	}





	if (CrackmeCheckeaDosGuionesCambialosPor0s(mySerial) == -1)
	{
		fprintf(stderr, "[-] No había suficientes '-' en la cadena, deben ser 2\n");
		return -1;
	}
	fprintf(stdout, "[+] Numero de guiones correcto\n\n");

	punteroSerial = mySerial;
	for (int i = 0; i < 3; i++)
	{
		__try
		{
			fprintf(stdout, "[+] Cadena a contar caracteres: %s\n", punteroSerial);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			fprintf(stderr, "[-] Error al mostrar cadenas por pantalla...: %X\n", GetLastError());
		}
		if (CrackmeStrlen(punteroSerial) != 6)
		{
			fprintf(stderr, "[-] Longitud de cadena no correcta, debe ser 6\n");
			return -1;
		}
		punteroSerial += 0x7;
	}

	fprintf(stdout, "\n");

	punteroSerial = mySerial;

	fprintf(stdout, "[+] Cadena tras cambiar por posicion: ");
	for (int i = 0; i < 3; i++)
	{
		CrackmeSetCharset(punteroSerial);

		for (int j = 0; j < 0x7; j++)
			fprintf(stdout, " %02X ", (BYTE)punteroSerial[j]);

		punteroSerial += 0x7;
	}
	fprintf(stdout, "\n\n");


	punteroSerial = mySerial;
	for (int i = 0; i < 3; i++)
	{
		serialHash[i] = CrackmeHasheaCadenaSerial(punteroSerial);
		if (serialHash[i] == -1)
		{
			fprintf(stderr, "[-] Error al obtener hash del serial...\n");
			return -1;
		}
		fprintf(stdout, "[+] Serial hash %d: 0x%08X\n", i, serialHash[i]);

	}

	fprintf(stdout, "\n");

	serialHash[0] = ROL_DW(serialHash[0], 0x7);
	serialHash[1] = ROR_DW(serialHash[1], 0x9);
	serialHash[2] = ROR_DW(serialHash[2], 0xb);

	serialHash[0] = serialHash[0] ^ serialHash[1];
	serialHash[2] = serialHash[2] + serialHash[1];
	serialHash[2] = serialHash[2] - serialHash[0];

	fprintf(stdout, "[+] Modificados los Serial hash:\n0: 0x%08X\n1: 0x%08X\n2: 0x%08X\n\n", serialHash[0], serialHash[1], serialHash[2]);


	// ahora empezamos con el nombre
	CrackmeObtenBytesDeNombre(myNombre, bufferNombre);
	fprintf(stdout, "[+] Nombre introducido: %s\n", myNombre);
	fprintf(stdout, "[+] Buffer del nombre:\n");
	int j = 1;
	for (int i = 0; i < 0x60; i++)
	{
		fprintf(stdout, " %02X ", bufferNombre[i]);
		if ( (j % 8) == 0)
			fprintf(stdout, "\n");
		j += 1;
	}
	fprintf(stdout, "\n");

	hashNombre = CrackmeObtenHashDeNombre(bufferNombre);
	fprintf(stdout, "[+] Hash del nombre: 0x%08X\n", hashNombre);

	CrackmeObtenParValoresDeNombre(bufferNombre, parValoresNombre);

	fprintf(stdout, "[+] Par de valores del nombre: 0x%08X,0x%08X\n", parValoresNombre[0], parValoresNombre[1]);

	int contador = 0xa;

	sumaValoresBuffer = CrackmeSumaValoresBuffer(bufferNombre);

	// Keygen realizado por Andrewl, esta parte no he podido conseguirlo
	// debido a la complejidad del algoritmo de cifrado
	// por tanto esta parte NO ES MIA ni me apropio de ella
	DWORD d0, d1, d2, t;

	#define OKAY 0x59414B4F
	#define _GUY 0x5955475F

	decrypt(hashNombre, 0x6B79745F, OKAY, _GUY, &t, &d2, 0);
	decrypt(parValoresNombre[0], parValoresNombre[1], t, sumaValoresBuffer, &d0, &d1, 1);

	fprintf(stdout, "[+] Valores descifrados respecto al nombre:\n0: 0x%08X\n1: 0x%08X\n2: 0x%08X\n\n",d0,d1,d2);


	INVERSE_PCODE_40355B(&d0, &d1, &d2);

	fprintf(stdout, "[+] Valores arreglados:\n0: 0x%08X\n1: 0x%08X\n2: 0x%08X\n\n", d0, d1, d2);

	CHAR serial[] = "XXXXXX-XXXXXX-XXXXXX";
	base41_encode(d0, serial);
	base41_encode(d1, serial + 7);
	base41_encode(d2, serial + 14);
	fprintf(stdout,"[*] Serial: %s\n", serial);

    return 0;
}

