/*
 * 2015 Yes Challenge Make-A-Thon
 * 혹시 아두이노 PCB 위에 올릴 때 그냥 올리지 마시고 종이같은거 로 밑에 핀 부분들 덮고 올리세요
 * 합선나서 아두이노 정상작동 안할 수도 있어요
 */

#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial btSerial(14,15); //bt 통신 안되면 핀 서로 바꿔볼 것. A0 = 14, A1 = 15
Servo LEFT;
Servo RIGHT;
#define ROTATE_TIME  20000 //서보 회전 시간 20초 동안 20도까지 서서히 움직임.

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)
//mp3 핀 설정

#define SERVO_LEFT 5
#define SERVO_RIGHT 6
//서보 핀 설정

#define CARDCS 4     // Card chip select pin
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin
//핀 세팅 예제 data sheet 보고함..

Adafruit_VS1053_FilePlayer musicPlayer = 
  //breakout 방식으로 함수 호출해서 노래가 재생되는 동안 인터럽트 실행 안되서 다른 행동 불가.
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);

  
void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);
  //현재 블루투스 MAC Addr = 20:15:02:13:13:35 비밀번호 1234
  //블루투스 테스트하시려면 Bluetooth Controller 라는 어플 편해요.
  
  LEFT.attach(SERVO_LEFT);
  RIGHT.attach(SERVO_RIGHT);
  
  Serial.println("Adafruit VS1053 Simple Test");

  if (!musicPlayer.begin()) 
  { 
    //혹시나 Micro SD 카드 다른거 쓰게되면 FAT16 형식으로 포맷해서 파일 쓰셔야되요.
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    delay(500);
  }
  
  SD.begin(CARDCS);    // initialise the SD card
  musicPlayer.setVolume(20,20);  //첫 번째 파라미터 왼쪽 음향크기, 두 번째 파라미터 오른쪽 음향크기
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  musicPlayer.startPlayingFile("a.mp3");
  //musicPlayer.playFullFile("a.mp3");
  //노래 현재 지코(JTONG) - 날 들어있습니다.
  //micro sd카드에 넣고 싶은 노래 a.mp3로 이름바꿔서 넣으면 됨. 외부전원 받는 일반적인 스피커가 없어서 이어폰으로만 테스트했는데 MP3 모듈과 연결된 케이블 건드리면 재생 꺼짐..
  //노래가 끝난 뒤 메인루프 시작함.
}

int btValue;
unsigned long time__ = millis();  //일정 하나의 외부루프 시간 관리용 변수
unsigned long time___ = millis(); //일정 시간 안에서 일정 시간 간격으로 사용할 때 사용
unsigned int rotate = 0;
int DIR = -1; // 1 : 점점 높은 각도로 진행, -1 : 점점 낮은 각도로 진행.
int flag = 0; // 'a'를 입력받기 전까지 모터 진행 안함.

void loop() {
  // File is playing in the background
  btValue = btSerial.read();
  //왠만하면 delay() 함수 사용하지 말고 타이머 함수 사용 권장.. delay 쓰면 pwm파형, 일부 인터럽트 제외하고 딜레이 시간동안 아두이노 멈춤..
  if(btValue == 'a')
  {
    //단톡방에 보낸 안드로이드 블루투스 통신 예제에서 메세지 보내는 것과 btValue 값을 동일하게 해주면됨.
    //안드로이드에서 소켓통신 블루투스 채널 스트림으로 "a"이 오면 아두이노에서는 첫 글자만 따도 되므로 'a'로 받음
    DIR = -1 * DIR; //방향은 0 -> 20 까지 올리고 20 -> 0 까지 내리기 위함.
//    flag = 1;
    time__ = millis();  //버튼 눌리기 시작했을 때 부터 시간 측정
    
  }
  if(time__ + ROTATE_TIME < millis())
  {
    //a 입력이 있은 후에 20초 뒤의 루틴
  }
  
//    if(rotate > 20 || rotate < 0) //서보 객체의 각값write는 음수값이 들어갈 수 없음.
//    {                             //
//      flag = 0;
//    }
//    
//    //20초 동안 1초에 1초씩 움직이게 함.
//    if(time___ + 1000 > millis())
//    {
//    rotate = rotate + ( DIR * 1 );
//    LEFT.write(rotate);
//    RIGHT.write(rotate);
//    }
//    else
//    time___ = millis();

  else
  {
    //20초 동안 루틴
    //20초 사이에 입력이 들어오게 할 것인가 말 것인가 결정해야됨.
  }
  
}
//주요기능, 요구사항 반영하여 추가할 사항 있으면 말해주세요
//서보 갈색선 = GND , 빨간선 = Vcc , 주황선 = pwm 신호선
//MP3 쉴드와 같이 쓰이는 단자에 스피커 잭 꽂고 확인해볼 수 있을듯..