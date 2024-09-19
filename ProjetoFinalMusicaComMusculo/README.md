# Projeto final de Sistemas Embarcados- Músculos Sonoros (PT)

O requisito do projeto desenvolvido como trabalho final da disciplina de Sistemas Embarcados feita por mim no 7° Semestre de Engenharia Biomédica na UNIFESP era aplciar todos os conhecimentos adquiridos na disciplina através da implementação de uma comunicação entre o microcontrolador Programmable Interface Controller (PIC) e o Arduino.

Para isso, resolvi juntar meus conhecimentos de obtenção de dados Eletromiográficos (EMG) e fazer um assistente lúdico para pacientes que precisariam fazer exercícios durante a fisioterapia. Assim, através de uma decodificação simples, seria possível ver quando o paciente contraiu o músculo e, neste momento, tocaria uma nota da música tema do filme Harry Potter. 
Desta forma, o projeto que implementei visa interpretar quando o sinal analógico lido pelo arduino passa de um limiar e tocar uma música quando este limiar é ultrapassado. Tudo isso, através de uma interface interativa feita graças a comunicação UART entre Arduino e PIC.

Os códigos para implementar os "Músculos Sonoros" estão dividos neste repositório entre o código do PIC e o código do Arduino.


# Final Project of Embedded Systems - Musical Muscles (ENG)

The requirement for the project developed as the final work for the Embedded Systems course, which I completed in the 7th semester of Biomedical Engineering at UNIFESP, was to apply all the knowledge acquired during the course through the implementation of communication between the Programmable Interface Controller (PIC) microcontroller and the Arduino.

For this, I decided to combine my knowledge of Electromyographic (EMG) data acquisition to create a playful assistant for patients who need to perform exercises during physiotherapy. In this way, through simple decoding, it would be possible to detect when the patient contracted a muscle, and at that moment, a note from the Harry Potter movie theme song would play.

Thus, the project I implemented aims to interpret when the analog signal read by the Arduino exceeds a threshold, and to play a song when this threshold is surpassed. All of this is done through an interactive interface enabled by UART communication between the Arduino and PIC.

The codes to implement "Musical Muscles" are divided in this repository between the PIC code and the Arduino code.
