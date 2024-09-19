# Projeto final de Sistemas Embarcados- Músculos Sonoros

O requisito do projeto desenvolvido como trabalho final da disciplina de Sistemas Embarcados feita por mim no 7° Semestre de Engenharia Biomédica na UNIFESP era aplciar todos os conhecimentos adquiridos na disciplina através da implementação de uma comunicação entre o microcontrolador Programmable Interface Controller (PIC) e o Arduino.

Para isso, resolvi juntar meus conhecimentos de obtenção de dados Eletromiográficos (EMG) e fazer um assistente lúdico para pacientes que precisariam fazer exercícios durante a fisioterapia. Assim, através de uma decodificação simples, seria possível ver quando o paciente contraiu o músculo e, neste momento, tocaria uma nota da música tema do filme Harry Potter. 
Desta forma, o projeto que implementei visa interpretar quando o sinal analógico lido pelo arduino passa de um limiar e tocar uma música quando este limiar é ultrapassado. Tudo isso, através de uma interface interativa feita graças a comunicação UART entre Arduino e PIC.

Os códigos para implementar os "Músculos Sonoros" estão dividos neste repositório entre o código do PIC e o código do Arduino.
