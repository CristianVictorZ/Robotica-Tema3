# Introducere in Robotica - Tema3
Această temă reprezintă un joc de tip QuickTime. Fiecare jucător va avea butoane și LED-uri proprii, iar jocul se va desfășura în mai multe runde. Scopul fiecărui jucător este să apese cât mai rapid butonul care corespunde culorii afișate pe LED-ul RGB al echipei sale. Punctajul fiecărui jucător va fi afișat pe un ecran LCD și se va actualiza pe parcursul jocului. La finalul jocului, jucătorul cu cel mai mare punctaj este declarat câștigător.

## Componente
6x LED-uri (2 grupuri de câte 3 leduri, în cadrul unui grup trebuie să avem culori diferite)

2x LED RGB (1 pentru fiecare jucător)

6x butoane (3 pentru fiecare jucător)

1x LCD

1x servomotor

2x Breadboard

Fire de legatura

2x Arduino Uno

22x Rezistoare (1x 100 ohm, 12x 330 ohm, 9x 220 ohm)

1x Buzzer

## Schema

![Schema](https://github.com/user-attachments/assets/730b5022-dda5-4bc6-b0ff-429cfeb29473)

## Descriere

Jocul pornește cu afișarea unui mesaj de bun venit pe LCD. Apăsarea lunga a oricărui buton declanșează startul jocului. La început, vor trebui introduse în terminal numele celor 2 jucători (4 caractere fiecare). Fiecare jucător are trei butoane, fiecare asociat unui LED de o culoare diferită și un al 4-lea LED RGB. La fiecare rundă, fiecare jucător este cel activ. LED-ul RGB al jucătorului activ se aprinde într-o culoare corespunzătoare unuia dintre butoanele sale. Jucătorul trebuie să apese cât mai rapid butonul care corespunde culorii LED-ului RGB, pentru a obține puncte. Cu cât reacționează mai repede, cu atât primește mai multe puncte. La apăsarea unui buton, in funcție de dacă acesta a fost cel corect sau nu, buzzer-ul va genera un sunet pentru a indica acest lucru. La finalul unei runde LCD-ul afișează punctajul actualizat al ambilor jucători. Pe tot parcursul jocului display-ul LCD va arata punctajul fiecărui jucător. Servomotorul se rotește pe parcursul jocului, indicând progresul. O rotație completă a servomotorului marchează sfârșitul jocului. La final, LCD-ul afișează numele câștigătorului și scorul final pentru câteva secunde, apoi revine la ecranul de start cu mesajul de bun venit.

## Montaj

![IMG_20241117_191247](https://github.com/user-attachments/assets/9fb12a75-4d76-4de5-8ff1-6fd11ae6a542)

# Video

[<img src="https://img.youtube.com/vi/5c6IaWx1PC0/maxresdefault.jpg">](https://youtu.be/5c6IaWx1PC0)
