const { spawn } = require('child_process');
const path = require('path');
const SMB = require('../build/Release/shared-memory-buffer.node');

// Устанавливаем буфер для идентификатора 'childProcess1'
const buffer = Buffer.from('Hello from MAIN PROCESS');
SMB.setBuffer('childProcessSharedBuffer', buffer);

// Запускаем дочерний процесс
const child = spawn('node', [path.join(__dirname, 'child.js')]);

// Перехватываем данные из stdout дочернего процесса
child.stdout.on('data', (data) => {
  console.log('Child process data:', data.toString())
});

// Перехватываем ошибки, если они возникают
child.on('error', (err) => {
  console.error('Child process error:', err);
});

// Ожидаем завершения дочернего процесса
child.on('close', () => {
  // Очищаем буфер для идентификатора 'childProcess1'
  SMB.clearBuffer('childProcessSharedBuffer');
  console.log('Buffer for childProcessSharedBuffer cleared');
});
