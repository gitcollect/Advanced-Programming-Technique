#include <stdint.h>
#include <iostream>
#include <time.h>
#include <cmath>
#include "gmp.h"
using namespace std;

int main()
{
	mpz_t p, q, n, t, d, e, pad;
    gmp_randstate_t r_state;
	gmp_randinit_default(r_state);
	gmp_randseed_ui(r_state, time(NULL));
	
	//mpz_t plaintext, cyphertext, decrypted;
	//mpz_init(plaintext);
	//mpz_init(cyphertext);
	//mpz_init(decrypted);
	
	for(int i = 0; i < 6; i++)
	{
		unsigned long sz = 32 * pow(2, i);
	    //cout << "Generating keys of size " << sz << " bits." << endl;
		mpz_init(pad);
    	mpz_init(p);
		mpz_init(q);
		mpz_init(n);
		mpz_init(t);
		mpz_init(d);
		//mpz_init2(e, sz * 2);
		mpz_ui_pow_ui(pad, 2, sz);
		mpz_init_set_ui(e, 65537);	//set e

		mpz_urandomb(p, r_state, sz);
		mpz_urandomb(q, r_state, sz);
		mpz_add(p, p, pad);
		mpz_add(q, q, pad);	
		//mpz_add(q, p, q);
		//mpz_urandomb(e, r_state, sz * 2);
		//mpz_add(e, p, e);

  		mpz_nextprime(p, p);		//get p
   		mpz_nextprime(q, q);   	//get q
		//mpz_nextprime(e, e);
   		mpz_mul(n, p, q);      //n = p * q    

  		mpz_sub_ui(p, p, 1);
   		mpz_sub_ui(q, q, 1);
   		mpz_mul(t, p, q);    
   		mpz_invert(d, e, t);   //get d;
	   	
		mpz_t plaintext, cyphertext, decrypted;
		mpz_init2(plaintext, sz * 2);
		mpz_init2(cyphertext,sz * 2);
		mpz_init2(decrypted, sz * 2);
		
		mpz_urandomb(plaintext, r_state, sz * 2);
     	mpz_powm(cyphertext, plaintext, d, n);                   //Encrypt
     	mpz_powm(decrypted, cyphertext, e, n);				   //Decrypt
     	if(mpz_cmp(plaintext, decrypted) != 0) 	cout << "Computation ERROR" << endl;
			//cout << endl << n[j] << endl;
			//mpz_clear(p[j]);
			//mpz_clear(q[j]);
			//mpz_clear(n[j]);
			//mpz_clear(t[j]);
			//mpz_clear(d[j]);
			//mpz_clear(e[j]);
     	mpz_add_ui(pad, p, 1);
		cout << "mpz_class p" << sz << "(\"" << 	pad  	<< "\");" <<endl;
		mpz_add_ui(pad, q, 1);	
		cout << "mpz_class q" << sz << "(\"" << 	pad  	<< "\");" <<endl;
		cout << "mpz_class n" << sz << "(\"" << 	 n  	<< "\");" <<endl;
		cout << "mpz_class d" << sz << "(\"" << 	 d  	<< "\");" <<endl;
		cout << "mpz_class e" << sz << "(\"" << 	 e  	<< "\");" <<endl;
		cout << "mpz_class M" << sz << "(\"" <<  plaintext  << "\");" <<endl;	
		cout << "mpz_class C" << sz << "(\"" <<  cyphertext << "\");" <<endl;
		//cout << "mpz_class D" << sz << "(\"" <<  decrypted  << "\");" <<endl;


		mpz_clear(plaintext);
		mpz_clear(cyphertext);
		mpz_clear(decrypted);
		
		/*
		if(sz == 64) {
			mpz_sqrt(q, n);
			//cout << endl << q << endl;
			do {
				mpz_nextprime(q, q);
				cout << q << endl;
				mpz_mod(pad, n, q);
				if(mpz_cmp_ui(pad, 0) == 0)
				{
					cout << "____________________mpz_class cracked q" << sz << "(\"" << 	q  	<< "\");" <<endl;
					break;
				}
			} while (1);
		}*/
	}
	//mpz_clear(plaintext);
	//mpz_clear(cyphertext);
	//mpz_clear(decrypted);
	gmp_randclear(r_state);
}
