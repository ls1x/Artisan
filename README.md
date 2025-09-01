# Artisan

### Description
**Artisan** is a simple and lightweight port scanner written in C.  
It can be used in penetration testing exercises, Capture the Flag (CTF) challenges, or any other environments where you have explicit permission to scan.

### Features
- Fast TCP port scanning  
- Customizable target and port range  
- Minimal and portable C implementation  
- Clear output to identify open/closed ports  

### Compilation
To compile the project, use `gcc`:
- `gcc main.c functions.c -o artisan`

### Usage
- `./artisan -h <IP> -p <PORT>`
- `./artisan -h 127.0.0.1 -p 80`
