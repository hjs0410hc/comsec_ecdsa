#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/sha.h>



using namespace std;
typedef long long ll;


EC_KEY* generate_ecdsa_key_pair(){
    EC_KEY *eckey = EC_KEY_new_by_curve_name(NID_secp256k1);    // EC_KEY 내 타원곡선 group 지정
    if(eckey == NULL){
        cerr << "Error occurred during allocating new ECKey." << endl;
        abort();
    }
    if(!EC_KEY_generate_key(eckey)){            // EC_KEY 내 비밀키와 공개키 생성
        cerr << "Error occurred during generating ECKey pair." << endl;
        abort();
    }
    return eckey;
}

ECDSA_SIG* ecdsa_sign(const char* message, EC_KEY* eckey){
    SHA256_CTX c;
    ECDSA_SIG *sig; // ECDSA 서명의 포인터
    unsigned char m[SHA256_DIGEST_LENGTH];

    // SHA256
    SHA256_Init(&c);
    SHA256_Update(&c,message,sizeof(message));
    SHA256_Final(m,&c);
    OPENSSL_cleanse(&c,sizeof(c));

    sig=ECDSA_do_sign(m,SHA256_DIGEST_LENGTH,eckey);    // eckey 내 private key로 SHA256 DIGEST를 서명
    if(sig==NULL){
        cerr<<"Error occurred during ECDSA Signing." << endl;
        abort();
    }

    return sig;
}

bool ecdsa_verify(const char* message, EC_KEY* eckey, ECDSA_SIG* sig){
    SHA256_CTX c;
    unsigned char m[SHA256_DIGEST_LENGTH];

    // SHA256
    SHA256_Init(&c);
    SHA256_Update(&c,message,sizeof(message));
    SHA256_Final(m,&c);
    OPENSSL_cleanse(&c,sizeof(c));

    int result = ECDSA_do_verify(m,SHA256_DIGEST_LENGTH,sig,eckey); // 인자로 받은 message의 SHA256 digest와 sig를 비교: verification
    return ((result == 1) ? true : false);
}

void test_ecdsa(const char* originalmessage, const char* testmessage){
    EC_KEY* eckey = generate_ecdsa_key_pair();                  // 타원곡선 키 생성
    ECDSA_SIG* sig = ecdsa_sign(originalmessage,eckey);         // originalmessage를 eckey내 private key로 서명
    cout << "Original Message: " << originalmessage << "\n";    
    cout << "Test Message: " << testmessage << "\n";

    if(ecdsa_verify(testmessage,eckey,sig)){                    // testmessage와 sig를 eckey 내 public key를 이용하여 인증.
        cout << "Successfully verified." << endl;
    }else{
        cout << "Verification FAILED." << endl;
    }
    ECDSA_SIG_free(sig);    // 메모리 누수 방지
    EC_KEY_free(eckey);     // 메모리 누수 방지
}

char argv1[256]; // 256 글자로 제한
char argv2[256]; // 256 글자로 제한
int main(int argc, char** argv){
    if(argc == 3){
        // argv[1] = original message
        // argv[2] = test message
        strncpy(argv1,argv[1],strlen(argv[1]));
        strncpy(argv2,argv[2],strlen(argv[2]));

        test_ecdsa(argv1,argv2);                        // 테스트 함수 실행.
    }else{
        cerr << "wrong argument given." << endl;
        abort();
    }
    
}