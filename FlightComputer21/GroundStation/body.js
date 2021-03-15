// Scrip goes into index.html
// Test for using google chrome's low power bluetooth api to receive data from FCC
// Documentation can be found at https://web.dev/bluetooth/


var deviceName = 'Bluefruit52'
var bleService = 'environmental_sensing'
var bleCharacteristic = 'temperature'
var bluetoothDeviceDetected
var gattCharacteristic

document.querySelector('#read').addEventListener('click', function() {
if (isWebBluetoothEnabled()) { read() }
})

document.querySelector('#start').addEventListener('click', function(event) {
if (isWebBluetoothEnabled()) { start() }
})

document.querySelector('#stop').addEventListener('click', function(event) {
if (isWebBluetoothEnabled()) { stop() }
})

function isWebBluetoothEnabled() {
if (!navigator.bluetooth) {
    console.log('Web Bluetooth API is not available in this browser!')
    document.getElementById("demo").innerHTML = 'Web Bluetooth API is not available in this browser!' ;

    return false
}

return true
}

function getDeviceInfo() {
let options = {
    optionalServices: [bleService],
    filters: [
    { "name": deviceName }
    ]
}

console.log('Requesting any Bluetooth Device...')
return navigator.bluetooth.requestDevice(options).then(device => {
    bluetoothDeviceDetected = device
}).catch(error => {
    console.log('Argh! ' + error)
    document.getElementById("demo").innerHTML = error ;

})
}

function read() {
return (bluetoothDeviceDetected ? Promise.resolve() : getDeviceInfo())
.then(connectGATT)
.then(_ => {
    console.log('Reading UV Index...')
    return gattCharacteristic.readValue()
})
.catch(error => {
    console.log('Waiting to start reading: ' + error)
    document.getElementById("demo").innerHTML = 'Waiting ' + error ;

})
}

function connectGATT() {
if (bluetoothDeviceDetected.gatt.connected && gattCharacteristic) {
    return Promise.resolve()
}

return bluetoothDeviceDetected.gatt.connect()
.then(server => {
    console.log('Getting GATT Service...')
    return server.getPrimaryService(bleService)
})
.then(service => {
    console.log('Getting GATT Characteristic...')
    return service.getCharacteristic(bleCharacteristic)
})
.then(characteristic => {
    gattCharacteristic = characteristic
    gattCharacteristic.addEventListener('characteristicvaluechanged',
        handleChangedValue)
    document.querySelector('#start').disabled = false
    document.querySelector('#stop').disabled = true
})
}

function handleChangedValue(event) {
let value = gattCharacteristic.value.getUint8()
var now = new Date()
console.log('> ' + now.getHours() + ':' + now.getMinutes() + ':' + now.getSeconds() + ' Temperature: ' + value + '[C]')
document.getElementById("demo").innerHTML = value;
}

function start() {
gattCharacteristic.startNotifications()
.then(_ => {
    console.log('Start reading...')
    document.querySelector('#start').disabled = true
    document.querySelector('#stop').disabled = false
})
.catch(error => {
    console.log('[ERROR] Start: ' + error)
})
}

function stop() {
gattCharacteristic.stopNotifications()
.then(_ => {
    console.log('Stop reading...')
    document.querySelector('#start').disabled = false
    document.querySelector('#stop').disabled = true
})
.catch(error => {
    console.log('[ERROR] Stop: ' + error)
})
}