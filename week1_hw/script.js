console.log("Hello World from the script!");

let portSelector;
let serial;

const synth = window.speechSynthesis;
let utterThis;
let volume = 0.5;
let pitch = 1;
let rate = 1;

function setup(event) {
    button = document.getElementById('my-button');
    button.addEventListener('click', buttonClicked);

    slider = document.getElementById('slider1');
    slider.addEventListener('change', setSpeechVolume);

    slider2 = document.getElementById('slider2');
    slider2.addEventListener('change', setSpeechPitch);

    slider3 = document.getElementById('slider3');
    slider3.addEventListener('change', setSpeechRate);

    serial = new p5.SerialPort();
    serial.on('list', printList);
    serial.on('data', serialEvent);
    serial.list();

    setSpeechVolume();
    setSpeechPitch();
    setSpeechRate();
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

function setSpeechVolume(e) {
    console.log(e);
    var currentValue = e;
    if (typeof e == 'object') {
        currentValue = e.target.value;
    }
    let thisSpan = document.getElementById('slider1');
    thisSpan.innerHTML = currentValue;
    console.log("Volume Slider is adjusted!");
    volume = slider.value * 0.1;
    speak();
}

function setSpeechPitch(f) {
    console.log(f);
    var currentValue2 = f;
    if (typeof f == 'object') {
        currentValue2 = f.target.value;
    }
    let thisSpan2 = document.getElementById('slider2');
    thisSpan2.innerHTML = currentValue2;
    console.log("Pitch Slider is adjusted!");
    pitch = slider2.value * 0.1;
    speak();
}

function setSpeechRate(g) {
    console.log(g);
    var currentValue3 = g;
    if (typeof g == 'object') {
        currentValue3 = g.target.value;
    }
    let thisSpan3 = document.getElementById('slider3');
    thisSpan3.innerHTML = currentValue3;
    console.log("Rate Slider is adjusted!");
    rate = slider3.value * 0.1;
    speak();
}

function printList(portList) {
    portSelector = document.getElementById('portSelector');
    for (var i = 0; i < portList.length; i++) {
        var option = document.createElement("option");
        option.text = portList[i];
        portSelector.add(option);
    }
    portSelector.addEventListener('change', openPort);
}

function openPort() {
    let item = portSelector.value;
    if (serial.serialport != null) {
        serial.close();
    }
    serial.open(item);
    console.log("open" + item);
}

function serialEvent() {
    var inData = serial.readLine();
    if (inData) {
        var sensors = JSON.parse(inData);
        console.log(sensors);
        setSpeechVolume(sensors.knob1);
        setSpeechPitch(sensors.knob2);
        setSpeechRate(sensors.knob3);
    }
}

window.addEventListener('DOMContentLoaded', setup);