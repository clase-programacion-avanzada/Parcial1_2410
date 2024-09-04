# Solución Parcial 1 2410

## Enunciado del parcial

¡Saludos, valiente programador! Te damos la bienvenida al desafío definitivo propuesto por HellBank, el banco de las profundidades, donde las riquezas y secretos del inframundo esperan ser gestionados con tu talento. Este banco único en su especie te invita a desarrollar una herramienta crucial para procesar archivos .hbt (Hel Bank Transactions). Estos archivos, escenciales para el funcionamiento del banco, son un compendio binario de las transacciones realizadas en cuentas específicas y requieren de tu habilidad para ser decodificados.

Un archivo. hbt representa las transacciones de una única cuenta y organiza su información en el siguiente formato binario:

- Nombre del Cliente (20 bytes): Codifica el nombre del cliente en 20 caracteres. Si el nombre es más corto, se completa con espacios.</br></br>
- Número de Cuenta (10 bytes): Almacena el número de la cuenta bancaria como una cadena de 10 dígitos numericos.</br></br>
- Tipo de Cuenta (1 byte): Indica el tipo de cuenta: 'A corresponde a una cuenta de ahorros, mientras que 'C identifica una cuenta corriente.</br></br>
- Número de Transacciones (1 byte): Especifica la can- tidad de transacciones que se detallan a continuación en el archivo.</br></br>
- Transacciones (variable): Cada transaccion se inicia
con un byte que marca el tipo ( C ' para crédito, 'D para débito), seguido de 8 bytes que representan el monto de la transacción en centavos, almacenado co- mo un valor long.

Desarrollarás un programa en C++ que pueda leer estos archivos hbt, procesando la información para calcular y mostrar:

- Nombre del Cliente.
- Número de Cuenta.
- Tipo de Cuenta.
- Saldo Total en la Cuenta (sumando créditos y restando débitos).
- Totales de Créditos y Débitos.
- Número de Transacciones Procesadas.

Basado en esta idea, implemente los siguientes puntos Para esto:

1. (5 puntos) Defina una estructura que represente una transacción según la especificación del archivo .hbt mencionado previamente. Esta estructura sólo debe contener el tipo (crédito o débito) y el monto.</br></br>
2. (5 puntos) Defina una estructura que represente la información de un cliente según la especificación del archivo hbt mencionado previamente. Esta estructura debe incluir el nombre del cliente, el número de cuenta, el tipo de cuenta, el número de transacciones de la cuenta, y un arreglo dinámico de transacciones usando la estructura del punto anterior.</br></br>
3. (35 puntos) Desarrolle una función que lea un archivo binario .hbt y retorne una estructura como la definida en el punto anterior con toda la información del archivo.</br></br>
4. (20 puntos) Desarrolle una función que reciba como parámetros un arreglo dinámico de transacciones, el tamaño del arreglo, y un tipo de transacción ('C' o 'D'). La función debe retornar la sumatoria de los montos de ese tipo de transacción.</br></br>
5. (5 puntos) Desarrolle una función que reciba como parámetros un arreglo dinámico de transacciones y el tamaño del arreglo, y retorne el saldo de la cuenta. El saldo de la cuenta se calcula a partir de los créditos y débitos sobre al cuenta asumiendo un saldo inicial de 0 centavos.</br></br>
6. (10 puntos) Desarrolle una función que reciba como parámetro una estructura como la definida en el punto 2 y la ruta a un archivo de texto. Esta función debe escribir la información de la cuenta en el archivo, como se muestra en el siguiente ejemplo:
    ```
        Cliente: Juan Arroyo 
        Cuenta: 1234567890
        Tipo de Cuenta: Ahorros 
        Total en Cuenta: X pesos 
        Total Créditos: Y pesos
        Total Débitos: z pesos
        Número de Transacciones en Archivo: 10
    ```
</br>
¡Acepta el reto de HellBank! Tu habilidad para descifrar y gestionar estas transacciones binarias es crucial para mantener el flujo de las riquezas en el inframundo. ¡Buena suerte!

(Restricciones: No se puede usar el tipo de datos string, los arreglos deben ser dinámicos, y recorrerse con aritmética de apuntadores.)