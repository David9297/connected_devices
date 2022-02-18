/*Source code: 
https://github.com/tigoe/html-for-conndev/blob/main/bluetooth-p5ble/ReadOneCharacteristic/script.js
*/

const serviceUuid = "19b10010-e8f2-537e-4f6c-d104768a1214";
let myCharacteristic;
let myBLE;

let connectButton;

const synth = window.speechSynthesis;
let utterThis;
let volume = 0.5;
let pitch = 1;
let rate = 1;

let lastValue1 = 0;
let lastValue2 = 0;
let lastValue3 = 0;

function setup(event) {
    button = document.getElementById('my-button');
    button.addEventListener('click', buttonClicked);

    slider = document.getElementById('slider1');
    slider.addEventListener('change', setSpeechVolume);

    slider2 = document.getElementById('slider2');
    slider2.addEventListener('change', setSpeechPitch);

    slider3 = document.getElementById('slider3');
    slider3.addEventListener('change', setSpeechRate);

    myBLE = new p5ble();

    connectButton = document.getElementById('connect');
    connectButton.addEventListener('click', connectToBle);

    setSpeechVolume();
    setSpeechPitch();
    setSpeechRate();
}

function connectToBle() {
    myBLE.connect(serviceUuid, gotCharacteristics);
}

function gotCharacteristics(error, characteristics) {
    if (error) {
        console.log(error);
        return;
    }

    console.log('characteristics: ', characteristics);
    myCharacteristic = characteristics[0];
    myBLE.read(myCharacteristic, 'string', gotValue);
}

function gotValue(error, value) {
    if (error) {
        return;
    }

    let values = value.split(":");
    let value1 = values[0];
    let value2 = values[1];
    let value3 = values[2];

    if (value1) {
        slider.value = value1;
        if (Math.abs(value1 - lastValue1) > 0.1) {
            setSpeechVolume(value1);
        }
        lastValue1 = value1;
    }

    if (value2) {
        slider2.value = value2;
        if (Math.abs(value2 - lastValue2) > 0.1) {
            setSpeechPitch(value2);
        }
        lastValue2 = value2;
    }

    if (value3) {
        slider3.value = value3;
        if (Math.abs(value3 - lastValue3) > 0.1) {
            setSpeechRate(value3);
        }
        lastValue3 = value3;
    }

    myBLE.read(myCharacteristic, 'string', gotValue);
}

function buttonClicked() {
    console.log("button clicked");
    speak();
}

function speak() {
    if (synth.speaking) {
        console.error("it's speaking already");
        return;
    }
    let textInput = document.querySelector("#text-input").value;
    utterThis = new SpeechSynthesisUtterance(textInput);
    utterThis.volume = volume;
    utterThis.pitch = pitch;
    utterThis.rate = rate;
    console.log(utterThis.volume, utterThis.pitch, utterThis.rate);
    synth.speak(utterThis);
}

function setSpeechVolume(value1) {
    console.log(value1);
    var currentValue = value1;
    if (typeof value1 == 'object') {
        currentValue = value1.target.value;
    }
    let thisSpan = document.getElementById('slider1');
    thisSpan.value = currentValue;
    console.log("Volume Slider is adjusted!");
    volume = slider.value * 0.1;
    speak();
}

function setSpeechPitch(value2) {
    console.log(value2);
    var currentValue2 = value2;
    if (typeof value2 == 'object') {
        currentValue2 = value2.target.value;
    }
    let thisSpan2 = document.getElementById('slider2');
    thisSpan2.value = currentValue2;
    console.log("Pitch Slider is adjusted!");
    pitch = slider2.value * 0.1;
    speak();
}

function setSpeechRate(value3) {
    console.log(value3);
    var currentValue3 = value3;
    if (typeof value3 == 'object') {
        currentValue3 = value3.target.value;
    }
    let thisSpan3 = document.getElementById('slider3');
    thisSpan3.value = currentValue3;
    console.log("Rate Slider is adjusted!");
    rate = slider3.value * 0.1;
    speak();
}

window.addEventListener('DOMContentLoaded', setup);