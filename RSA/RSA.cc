#include <stdint.h>
#include <iostream>
#include <time.h>
#include <cmath>
#include "gmp.h"

using namespace std;

int main()
{
	mpz_t p, q, n, t, d, e, pad, crp32, crq32;
    gmp_randstate_t r_state;
	gmp_randinit_default(r_state);
	gmp_randseed_ui(r_state, time(NULL));
	
	mpz_t plaintext, cyphertext, decrypted;
	mpz_init(plaintext);
	mpz_init(cyphertext);
	mpz_init(decrypted);
	mpz_init(crp32);
	mpz_init(crq32);
	
	for(int i = 0; i < 6; i++)
	{
		unsigned long sz = 32 * pow(2, i);
		mpz_init(pad);
    	mpz_init(p);
		mpz_init(q);
		mpz_init(n);
		mpz_init(t);
		mpz_init(d);
		mpz_init(e);
		mpz_ui_pow_ui(pad, 2, sz - 1);
		do {
			
			mpz_urandomb(p, r_state, sz);
			mpz_urandomb(q, r_state, sz);
  			mpz_nextprime(p, p);		//get p
   			mpz_nextprime(q, q);   	//get q
   			mpz_mul(n, p, q);      //n = p * q    
		} while (mpz_sizeinbase(n, 2) != sz * 2);

		mpz_urandomb(e, r_state, mpz_sizeinbase(n, 2) - 1);
   		mpz_nextprime(e, e);   	//get q
  		
		mpz_sub_ui(p, p, 1);
   		mpz_sub_ui(q, q, 1);
   		mpz_mul(t, p, q);    
   		mpz_invert(d, e, t);   //get d;
	   	
		mpz_t plaintext, cyphertext, decrypted;
		mpz_init2(plaintext, sz * 2);
		mpz_init2(cyphertext,sz * 2);
		mpz_init2(decrypted, sz * 2);
		mpz_urandomb(plaintext, r_state, mpz_sizeinbase(n, 2) - 1);
		mpz_powm(cyphertext, plaintext, d, n);                   //Encrypt
     	mpz_powm(decrypted, cyphertext, e, n);				   //Decrypt
     	mpz_add_ui(pad, p, 1);
		cout << "mpz_class p" << sz << "(\"" << 	pad  	<< "\");" <<endl;
		mpz_add_ui(pad, q, 1);
		cout << "mpz_class q" << sz << "(\"" << 	pad  	<< "\");" <<endl;
		cout << "mpz_class n" << sz << "(\"" << 	 n  	<< "\");" <<endl;
		cout << "mpz_class d" << sz << "(\"" << 	 d  	<< "\");" <<endl;
		cout << "mpz_class e" << sz << "(\"" << 	 e  	<< "\");" <<endl;
		
		cout << "mpz_class M" << sz << "(\"" <<  decrypted  << "\");" <<endl;	
		cout << "mpz_class C" << sz << "(\"" <<  cyphertext << "\");" <<endl;
		if(sz == 32) {
			mpz_sqrt(crq32, n);
			do {
				mpz_nextprime(crq32, crq32);
				mpz_mod(pad, n, crq32);
				if(mpz_cmp_ui(pad, 0) == 0)
				{
					mpz_fdiv_q(crp32, n, crq32);
					break;
				}
			} while (1);
			cout << "mpz_class p32Attack(\"" <<  crp32  << "\");" <<endl;
			cout << "mpz_class q32Attack(\"" <<  crq32  << "\");" <<endl;	
			cout << "mpz_class n32Attack(\"" <<    n    << "\");" <<endl;	
			
			mpz_sub_ui(crq32, crq32, 1);
			mpz_sub_ui(crp32, crp32, 1);
			mpz_mul(t, crq32, crp32);
			mpz_invert(e, d, t);
			cout << "mpz_class d32Attack(\"" <<     d   << "\");" <<endl;
			cout << "mpz_class e32Attack(\"" <<     e   << "\");" <<endl;	
     	    mpz_powm(decrypted, cyphertext, e, n);				   //Decrypt
			cout << "mpz_class M32Attack(\"" <<  decrypted   << "\");" <<endl;	
			cout << "mpz_class C32Attack(\"" <<  cyphertext  << "\");" <<endl;	
		}
		mpz_clear(plaintext);
		mpz_clear(cyphertext);
		mpz_clear(decrypted);
	}
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(t);
	mpz_clear(d);
	mpz_clear(e);
	mpz_clear(crq32);
	mpz_clear(crp32);
	mpz_clear(plaintext);
	mpz_clear(cyphertext);
	mpz_clear(decrypted);
	gmp_randclear(r_state);
}
