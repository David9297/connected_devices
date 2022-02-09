const giphyAPIKey = "VdYY0nULA3Sgji8X2rp66SalK7YxUzrA";

let qrDiv;

function getQrCode() {
    qrDiv = document.getElementById('qrCode');
    let qrText = document.getElementById('textField').value;
    fetchImg(qrText);
}

function fetchImg(phrase) {
    const url = `https://api.giphy.com/v1/gifs/random?api_key=${giphyAPIKey}&tag=${phrase}`;
    fetch(url, { mode: "cors" })
        .then(response => response.json()) 
        .then(data => getResponse(data))
        .catch(error => getResponse(error));
}

function getResponse(responseText) {
    console.log(responseText);
    if (typeof responseText == 'string') {
        console.log("it's a string");
    }
    console.log(responseText);
    let qr = qrcode(0, 'L');
    qr.addData(responseText.data.url);
    qr.make();
    let qrImg = qr.createImgTag(2, 8);
    qrDiv.innerHTML = qrImg;
}