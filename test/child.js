const SMB = require('../build/Release/shared-memory-buffer.node');
let receivedBuffer = "";
try {
  receivedBuffer = SMB.getBuffer('childProcessSharedBuffer');
} catch {

}
process.stdout.write(receivedBuffer);
