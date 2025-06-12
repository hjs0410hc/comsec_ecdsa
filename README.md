# comsec_ecdsa

OO대학교 컴퓨터공학과 컴퓨터보안 과목의 최종 과제입니다.

타원곡선 암호를 이용하여 ECDSA (전자서명) 기능을 구현했습니다.

OpenSSL 라이브러리가 사용되었습니다.

# REPORT

EC-DSA 설명

## 암호 라이브러리 설치 방법 (WSL Ubuntu 20.04.6 LTS 환경)

```sh
sudo apt install openssl
sudo apt install libssl-dev
```
위 명령어를 실행하면 openssl 1.1.1f가 설치되며 /usr/include에 openssl include 폴더가 생깁니다.
사용하는 헤더는 openssl/ec.h  openssl/evp.h  openssl/ecdsa.h 입니다.

## Key generation 방법 설명 (generate_ecdsa_key_pair)

사용하려는 타원곡선은 secp256k1으로 정했습니다. (키 길이 256비트)
EC_KEY_new_by_curve_name(타원곡선) 을 통해서 EC_KEY를 만들 수 있습니다. EC_KEY는 내부에 타원곡선 group, public key와 private key 등을 가질 수 있습니다.
다음 EC_KEY_generate_key(eckey)를 이용하여 EC_KEY 내부에 비밀키와 공개키를 생성합니다.
이제 준비가 다 된 EC_KEY*를 return합니다.

## Sign 방법 설명 (ecdsa_sign)

message와 EC_KEY*를 인자로 받습니다. SHA256을 해쉬함수로 사용할 것입니다. SHA256 Init-Update-Final 과정이 필요합니다. Update 때 message를 SHA256 digest로 바꾸게 됩니다. 이제 ECDSA_do_sign(digest, digestlength, eckey*) 함수를 이용하여 비밀키로 서명합니다. 나중에 verify할 때 ECDSA_SIG가 사용됩니다. 준비가 다 된 ECDSA_SIG*를 return합니다.

## Verify 방법 설명 (ecdsa_verify)

testmessage와 EC_KEY*, ECDSA_SIG*를 인자로 받습니다. SHA256을 또 해쉬함수로 사용해야 합니다. 다시 일련의 해쉬 과정을 거쳐 testmessage의 SHA256 digest를 확보합니다. 이제 ECDSA_do_verify(digest, digestlength, ECDSA_SIG*, EC_KEY*)를 이용, 서명과 공개키를 이용하여 verify합니다. 이때 verify가 실패 시 함수는 0을 반환, 성공 시 1을 반환합니다. 우리 함수는 이를 받아 true/false로 변환해서 반환해줄 것입니다.

## Test 설명 (test_ecdsa)

g++ ecdsacode.cpp -lcrypto -o ecdsatest.o 로 컴파일합니다. (openssl libcrypto가 반드시 컴파일 때 명시적으로 링크되어야 합니다)
./ecdsatest.o [originalmessage] [testmessage]		(메시지들은 256바이트로 제한되어야 합니다)
위와 같이 인자를 줘서 테스트합니다. 성공 시 Successfully verifed, 실패시 Verification FAILED가 출력됩니다.
 

## MY THOUGHTS

암호란 보이지 않아 느껴지지 않습니다. 그러나 암호는 계속 우리 주변에 실재하며 정말 중요한 요소입니다.  이번 기회를 통해 직접 실습을 하여 결과물을 눈으로 확인할 수 있어서 성취감이 들었습니다. 특히나 이번 기회에 C++에서 Public 라이브러리를 제대로 써볼 수 있었기 때문에 더욱 그 감동이 큽니다. 한 학기 동안 고생 많이 하셨습니다 교수님. 재밌는 과제 덕분에 즐겁게 종강을 할 수 있게 되었습니다.

chatGPT는 일체 사용하지 않았습니다.
