/* Si intende implementare un progetto di selezione di vari 
 * programmi utilizzando pulsanti non a ritenzione che servano 
 * per selezionare un determinato programma (su tre disponibili) 
 * e un pulsante di avvio che mandi in esecuzione il programma 
 * selezionato. I programmi sono tra loro mutuamente esclusivi. 
 * In fase di test, si possono selezionare tre diverse tipologie 
 * di giochi: nel primo caso si effettua il lancio di un dado 
 * virtuale che, attraverso tre led che rappresentano cifre di un 
 * numero binario, genera un numero compreso tra 1 e 6; nel secondo 
 * caso si manda in esecuzione un lampeggio sequenziale di luci 
 * colorate, nel terzo caso il gioco si basa su chi è più veloce a 
 * premere un pulsante. 
 * Capogruppo : Maurizio Mainardi
   Autori : Vittorio Burlenghi - Lorenzo Cotti - Ismael Konate - 
   Maurizio Mainardi - Alberto Sillani
   Versione : 1.0
 */

// Inclusione della libreria per i numeri del display
#include <D_NUMERI1.h>

// Inclusione della libreria per i numeri in binario con accensione dei led
#include <L_NUMERI.h>

// Definizione dei piedini di Arduino per i 3 led, i 2 pulsanti 
// e il display utilizzando solo la quarta cifra
# define pulsanteA 18
# define pulsanteB 19
# define led1 14
# define led2 15
# define led3 16
# define display_A 2
# define display_B 3
# define display_C 4
# define display_D 5
# define display_E 6
# define display_F 7
# define display_G 8
# define display_4 9

// Creazione oggetto per la gestione dei numeri sul display
D_NUMERI1 dis;

// Creazione oggetto per la gestione dei numeri in binario con accensione dei led
L_NUMERI led;


int giocoIniziato = 0;
int pressioneA = 0;
int pressioneB = 0;
bool giocatore1Premuto = 0;
bool giocatore2Premuto = 0;
long countdown = 0;
long tempoIniziale = 0;
long timerG1 = 0;
long timerG2 = 0;
long tempoPressioneG1 = 0;
long tempoPressioneG2 = 0;

// Variabile random per contenere il numero del dado
byte randNumber;

// Scelta del gioco
int scelta = 0; 

// Tempo di "reset" se nessun pulsante viene premuto
int tempo = 3; 
unsigned long currentMillis = 0;

void setup() {
  
  // Definizione dei pin dei 3 led come OUTPUT
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  // Definizione dei pin dei 2 pulsanti come INTPUT utilizzando
  // il PULLUP per non dover inserire resistenza fisiche
  pinMode(pulsanteA, INPUT_PULLUP);
  pinMode(pulsanteB, INPUT_PULLUP);

  // Definisco i pin del display come OUTPUT
  pinMode(display_A, OUTPUT);
  pinMode(display_B, OUTPUT);
  pinMode(display_C, OUTPUT);
  pinMode(display_D, OUTPUT);
  pinMode(display_E, OUTPUT);
  pinMode(display_F, OUTPUT);
  pinMode(display_G, OUTPUT);
  pinMode(display_4, OUTPUT);

  // Setta la velocità di comunicazione
  Serial.begin(9600);

  // Reset della numerazione random
  randomSeed(analogRead(3));

}

/* Programma principale per la scelta del gioco da avviare 
   Autori : Vittorio Burlenghi - Alberto Sillani
   Versione : 1.0
 */
void loop() {
  // Cooldown se il programma non viene premuto per 3 secondi si resetta la scelta
  if (((millis() - currentMillis) / 1000) >= tempo) { 
    // Spegnimento dei led
    led.l_spento(led3, led2, led1);
    // Spegnimento del display
    digitalWrite(display_4, LOW);
    dis.d_spento(display_A, display_B, display_C, display_D, display_E, display_F, display_G);
    scelta = 0;
  }
  // Pressione del pulsante A per la scelta del gioco da avviare
  if (digitalRead(pulsanteA) == LOW) {
    digitalWrite(display_4, HIGH);
    currentMillis = millis();
    scelta += 1;
    if (scelta == 1) {
      // Accensione dei led per il gioco 1
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);  
    }
    else if (scelta == 2) {
      // Accensione dei led per il gioco 2
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
    }
    else if (scelta == 3) {
      // Accensione dei led per il gioco 3
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, HIGH);
    }
    // Accensione di tutti i led per il gioco casuale
    else if (scelta == 4) {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
    }
    else {
      // Valore della scelta riparte da 1
      scelta = 1;
      // Accensione dei led per il gioco 1
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);      
    }
    // Accensione del display con il numero del gioco selezionato
      dis.d_acceso(display_A, display_B, display_C, display_D, display_E, display_F, display_G, scelta);
    delay(200);
    // Visualizzazione a monitor della scelta del gioco scelto
    if(scelta != 4)
    {
      Serial.print("Avvia il gioco : ");
      Serial.print(scelta);
      Serial.println(" -  Premi il pulsante B per avviare il gioco.");
    }
    // Visualizzazione a monitor della scelta del gioco random
    else
    {
      Serial.print("Avvia il gioco scelto randomicamente ");
      Serial.println(" -  Premi il pulsante B per far partire un programma a caso tra i 3 disponibili.");
    }
  }
  // Pressione del pulsante B per la conferma del gioco da avviare
  if (digitalRead(pulsanteB) == LOW) {
    delay(200);
    // Spegnimento del display
    digitalWrite(display_4, LOW);
    dis.d_spento(display_A, display_B, display_C, display_D, display_E, display_F, display_G);
    // Spegnimento dei led dopo la scelta
    led.l_spento(led3, led2, led1);
    delay(500);
    // Scelta del gioco random, a scelta viene assegnato un numero casuale per far partire uno dei programmi
    if(scelta == 4)  
    {
      // Funzione che genera la scelta del gioco casuale
      scelta = prograndom();
    }
    switch (scelta) {
      case 1:
        // Avvia il gioco N 1 (Dado)
        dado();
        scelta = 0;
        break;
      case 2:
        // Avvia il gioco N 2 (Sequenza luci colarate)
        sequenzaLuci();
        scelta = 0;
        break;
      case 3:
        // Avvia il gioco N 3 (Pulsante più veloce)
        pulsanteVeloce();
        scelta = 0;
        break;
      default:
        break;
    }
  }
}

/* Funzione lancio casuale del dado 
   Autore : Maurizio Mainardi
   Versione : 1.0
 */
void dado()
{
  Serial.println("Gioco del dado iniziato!");
  Serial.print("Lancio del dado");
  // Sequenza di luci che simulano il lancio del dado
  for (int i = led3; i>=led1; i--) {
    digitalWrite(i, HIGH);
    Serial.print(".");
    delay(200);
  }
  for (int j = led1; j<=led3; j++) {
    digitalWrite(j, LOW);
    Serial.print(".");
    delay(200);
  }
  delay(1000);
  Serial.println("");
  // Genera un numero random compreso tra 1 e 6 come le facce del dado
  randNumber = random (1, 7);
  // Stampo il numero generato a monitor
  Serial.print("Numero uscito : ");
  Serial.println(randNumber);
  // Confronto il numero generato per l'accensione dei led in binario
  switch (randNumber) {
    case 1 :
      // Accensione quarta cifra del display
      digitalWrite(display_4,HIGH);
      // Accensione cifra 1 sul display
      dis.d_acceso(display_A, display_B, display_C, display_D, display_E, display_F, display_G,randNumber);
      // Accensione cifra 1 in binario sui led
      led.l_uno(led3, led2, led1);
      break;
    case 2 :
      // Accensione quarta cifra del display
      digitalWrite(display_4,HIGH);
      // Accensione cifra 2 sul display
      dis.d_acceso(display_A, display_B, display_C, display_D, display_E, display_F, display_G,randNumber);
      // Accensione cifra 2 in binario sui led
      led.l_due(led3, led2, led1);
      break;  
    case 3 :
       // Accensione quarta cifra del display
      digitalWrite(display_4,HIGH);
      // Accensione cifra 3 sul display
      dis.d_acceso(display_A, display_B, display_C, display_D, display_E, display_F, display_G,randNumber);
      // Accensione cifra 3 in binario sui led
      led.l_tre(led3, led2, led1);
      break;  
    case 4 :
      // Accensione quarta cifra del display
      digitalWrite(display_4,HIGH);
      // Accensione cifra 4 sul display
      dis.d_acceso(display_A, display_B, display_C, display_D, display_E, display_F, display_G,randNumber);
      // Accensione cifra 4 in binario sui led
      led.l_quattro(led3, led2, led1);
      break; 
    case 5 :
      // Accensione quarta cifra del display
      digitalWrite(display_4,HIGH);
      // Accensione cifra 5 sul display
      dis.d_acceso(display_A, display_B, display_C, display_D, display_E, display_F, display_G,randNumber);
      // Accensione cifra 5 in binario sui led
      led.l_cinque(led3, led2, led1);
      break;
    case 6 :
      // Accensione quarta cifra del display
      digitalWrite(display_4,HIGH);
      // Accensione cifra 6 sul display
      dis.d_acceso(display_A, display_B, display_C, display_D, display_E, display_F, display_G,randNumber);
      // Accensione cifra 6 in binario sui led
      led.l_sei(led3, led2, led1);
      break;
    default:
       break;
  }
  // Attesa di 2 secondi per la visualizzazione del risultato
  delay(3000);
  // Spegnimento dei led
  led.l_spento(led3, led2, led1);
  // Spegnimento di tutti i segmenti del display
  dis.d_spento(display_A, display_B, display_C, display_D, display_E, display_F, display_G); 
  // Spegnimento quarta cifra del display
  digitalWrite(display_4, LOW); 
  Serial.println("Gioco del dado terminato!");  
}

/* Funzione sequenza luci colorate 
   Autore : Ismael Konate
   Versione : 1.0
 */
void sequenzaLuci() 
{
  Serial.println("Gioco della sequenza di luci iniziato!");
  Serial.println("Generazione della sequenza di luci");
  // Generazione casuale di una sequenza di luci
  digitalWrite(led1, HIGH);
  delay(100);
  digitalWrite(led1, LOW);
  delay(15);
  digitalWrite(led2, HIGH);
  delay(90);
  digitalWrite(led2, LOW);
  delay(15);
  digitalWrite(led3, HIGH);
  delay(90);
  digitalWrite(led3, LOW);
  delay(15);
  digitalWrite(led2, HIGH);
  delay(100);
  digitalWrite(led2, LOW);
  delay(15);
  digitalWrite(led1, HIGH);
  delay(100);
  digitalWrite(led1, LOW);
  delay(15);
  digitalWrite(led3, HIGH);
  delay(100);
  digitalWrite(led3, LOW);
  delay(15);
  for (int j = 0; j <= 3; j++) {
     for (int i = 14; i <= 16; i++) {
       digitalWrite(i, HIGH);
       delay(100);
       digitalWrite(i, LOW);
     }
     for (int i = 16; i > 13; i--) {
       digitalWrite(i, HIGH);
       delay(100);
       digitalWrite(i, LOW);
     }    
   }
   // Spegnimento dei led
   led.l_spento(led3, led2, led1);
   Serial.println("Gioco della sequenza di luci terminato!");
}

/* Funzione calcola chi è più veloce a premere il pulsante 
   Autore : Lorenzo Cotti
   Versione : 1.0
 */
void pulsanteVeloce()
{
  Serial.println("Gioco del pulsante iniziato!");
  giocoIniziato = 1;
  // Funzione semaforo che da l'avvio al gioco
  Semaforo();
  // Generazione di un numero casuale per il tempo di attesa del led per calcolare il più veloce
  countdown = random(3, 12);  
  // Trasformazione del numero casuale in secondi di attesa
  countdown = countdown * 1000; 
  delay(countdown);
  // Accensione del led per il calcolo del più veloce
  digitalWrite(led2, HIGH);
  // Calcolo il momento esatto (dall'inizio del programma) nel quale si accende il led
  tempoIniziale = millis();  
  // Attesa della pressione di entrambi i pulsanti 
  while (giocatore1Premuto == 0 || giocatore2Premuto == 0) 
  {
    pressioneA = digitalRead(pulsanteA);
    pressioneB = digitalRead(pulsanteB);

    if (pressioneA == 0 && giocatore1Premuto == 0)
    {
      // Calcolo in quale momento dall'inizio del programma il G1 ha schiacciato il pulsante
      timerG1 = millis(); 
      giocatore1Premuto = 1;
    }

    if (pressioneB == 0 && giocatore2Premuto == 0)
    {
      // Calcolo in quale momento dall'inizio del programma il G2 ha schiacciato il pulsante
      timerG2 = millis(); 
      giocatore2Premuto = 1;
    }
    // Partita finita quando entrambi i giocatori hanno premuto il pulsante
    if (giocatore1Premuto == 1 && giocatore2Premuto == 1) 
    {
      // Funzione che gestisce il vincitore
      finisciPartita();
    }
  }
  Serial.println("Gioco del pulsante terminato!");
  Reset();
}

/* Funzione che calcola quanto tempo ogni giocatore ha impiegato 
 * per premere il pulsante e ne decreta il vincitore 
   Autore : Lorenzo Cotti
   Versione : 1.0
 */
void finisciPartita()
{
  digitalWrite(led2, LOW);
  // Funzione che attende il calcolo del vincitore
  attesaCalcolo();
  // Calcolo il tempo dei due giocatori
  tempoPressioneG1 = timerG1 - tempoIniziale;
  tempoPressioneG2 = timerG2 - tempoIniziale;
  Serial.print("Tempo di pressione Giocatore 1 in millisecondi : ");
  Serial.println(tempoPressioneG1);
  Serial.print("Tempo di pressione Giocatore 2 in millisecondi : ");
  Serial.println(tempoPressioneG2);

  // Se G1 ha premuto il pulsante in minor tempo di G2, illumina LED di sinistra e accende il display
  // con il numero 1
  if (tempoPressioneG1 < tempoPressioneG2)
  {
    digitalWrite(led3, HIGH);
    Serial.println("HA VINTO IL GIOCATORE 1");
    digitalWrite(display_4, HIGH);
    dis.d_acceso(display_A, display_B, display_C, display_D, display_E, display_F, display_G,1);
    delay(3000);
    // Spegnimento del led 3 del giocatore 1
    digitalWrite(led3, LOW);
  }
  // Se G2 ha premuto il pulsante in minor tempo di G1, illumina LED di destra e accende il display
  // con il numero 2
  if (tempoPressioneG2 < tempoPressioneG1)
  {
    digitalWrite(led1, HIGH);
    Serial.println("HA VINTO IL GIOCATORE 2");
    digitalWrite(display_4, HIGH);
    dis.d_acceso(display_A, display_B, display_C, display_D, display_E, display_F, display_G,2);
    delay(3000);
    // Spegnimento del led 1 del giocatore 2
    digitalWrite(led1, LOW);
  }
}

/* Funzione semaforo di LED per avvisare che il gioco sta iniziando 
   Autore : Lorenzo Cotti
   Versione : 1.0
 */
void Semaforo()
{
  digitalWrite(led3, HIGH);
  delay(600);
  digitalWrite(led2, HIGH);
  delay(600);
  digitalWrite(led1, HIGH);
  delay(600);
  // Spegnimento dei led
  led.l_spento(led3, led2, led1);
}

/* Funzione gioco di luci in attesa del calcolo, puramente estetico 
   Autore : Lorenzo Cotti
   Versione : 1.0
 */
void attesaCalcolo()
{
  int counter = 0;
  int conta = 0;
  int ledPins[] = {14, 15, 16};
  for (counter = 0; counter < 4; counter++)
  {
    for (conta = 0; conta < 3; conta++)
    {
      digitalWrite(ledPins[conta], HIGH);
      delay(140);
      digitalWrite(ledPins[conta], LOW);
    }
    for (conta = 2; conta >= 0; conta--)
    {
      digitalWrite(ledPins[conta], HIGH);
      delay(140);
      digitalWrite(ledPins[conta], LOW);
    }
  }
}

/* Funzione che resetta tutte le variabili per poter ricomincare 
 * il gioco del pulsante più veloce
   Autore : Lorenzo Cotti
   Versione : 1.0
 */
void Reset()
{
  giocoIniziato = 0;
  pressioneA = 0;
  pressioneB = 0;
  giocatore1Premuto = 0;
  giocatore2Premuto = 0;
  countdown = 0;
  tempoIniziale = 0;
  timerG1 = 0;
  timerG2 = 0;
  tempoPressioneG1 = 0;
  tempoPressioneG2 = 0;
  // Spegnimento di tutti i segmenti del display
  dis.d_spento(display_A, display_B, display_C, display_D, display_E, display_F, display_G); 
  // Spegnimento quarta cifra del display
  digitalWrite(display_4, LOW); 
}

/* Funzione che genera un numero casuale tra 1, 2 e 3, per scegliere un programma a caso 
   Autore : Vittorio Burlenghi
   Versione : 1.0
 */
int prograndom()
{
  // Genera un numero random compreso tra 1 e 3 come il numero dei programmi disponibili
  randNumber = random (1, 4);
  Serial.print("Programma selezionato randomicamente:\nProgramma ");
  Serial.println(randNumber);
  // Ritorna il numero random generato
  return randNumber;
}
