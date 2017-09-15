# lcwebhook

A simple demonstration github webhook, which parses the JSON payload and outputs the zen message to a librecast channel.

## Installation

```
make
```

Add something like the following config to gladd.conf & `gladd reload`:
```
url plugin POST /github/ /usr/local/src/lcwebhook/src/lcwebhook #welcome
```
