# Movefile

Movefile is a rename implementation working over device (linux and BSD only).

## installation

```bash
npm install movefile
```

make and python are required to compile movefile.

## Using Movefile

```js
var mv = require('movefile');

mv.move('file/to/move', 'where/to/move');
```

