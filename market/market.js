/*! OSNAP Market production code */
////////////////////////////////////////////////////////////////////////////////
//  Model
////////////////////////////////////////////////////////////////////////////////

var market = {
    "apps": [{
        "id": 0,
        "title": "OSNAP(app)",
        "icon": "icon/osnap/icon128.png",
        "url": "apk/OsnapApp.apk",
        "about": "Generic app for testing OSNAP-compatible arduino products.",
        "deps": [50]
    }, {
        "id": 1,
        "title": "JacketApp",
        "icon": "icon/jacket/icon128.png",
        "url": "apk/Tshirt.apk",
        "about": "This is our main prototype and will showcase a lot features. It consists of a T-Shirt or sweater connected to a Lilypad Arduino that will feature several displays and indicators which will receive social data from the Android phone.",
        "deps": [0, 4, 5]
    }, ...{
        "id": 2,
        "title": "TemperatureApp",
        "icon": "icon/temp/icon128.png",
        "url": "apk/TemperatureApp.apk",
        "about": "This prototype will primarily showcase the communication from Arduino to Android using our libraries. The Arduino unit can read the ambient temperature and be able to send this info to an Android application, either on demand or at a set interval that can be chosen through settings in the application.",
        "deps": [50]
    }, {
        "id": 3,
        "title": "LedApp",
        "icon": "icon/led/icon128.png",
        "url": "",
        "about": "Huge \"about\" string :)",
        "deps": [50]
    }, {
        "id": 4,
        "title": "FacebookService",
        "icon": "icon/fb/icon128.png",
        "url": "apk/facebook-release.apk",
        "about": "Huge \"about\" string :)"
    }, {
        "id": 5,
        "title": "TwitterService",
        "icon": "icon/twitter/icon128.png",
        "url": "apk/twitter-release.apk",
        "about": "Huge \"about\" string :)"
    }, {
        "id": 50,
        "title": "Barcode Scanner <small>(Google Play)</small>",
        "icon": "icon/3rdparty/barcodescanner.png",
        "url": "https://play.google.com/store/apps/details?id=com.google.zxing.client.android",
        "about": "Scan barcodes on products then look up prices and reviews. You can also scan Data Matrix and QR Codes containing URLs, contact info, etc. Also share your contacts, apps, and bookmarks via QR Code. This is why contacts permissions are needed."
    }]
};

////////////////////////////////////////////////////////////////////////////////
//  Controller
////////////////////////////////////////////////////////////////////////////////

var isAndroid = ( navigator.userAgent.toLowerCase().indexOf("android") >= 0 );

function generateApp(app, market) {

    var outputHTML = "";
    outputHTML += "<li>";
    outputHTML += "    <h3><strong><a name='appId" + app.id + "'>" + app.title + "</a></strong></h3>";

    if (app.icon) {
        outputHTML += "    <p><img src='" + app.icon + "' />" + app.about + "</p>";
    } else {
        outputHTML += "    <p>" + app.about + "</p>";
    }

    outputHTML += "    <p id='downloadBar'><small>";
    if (app.url != "") {
        if (isAndroid) {
            outputHTML += "    <a class='download' href='" + app.url + "'>Install</a>";
        } else {
            outputHTML += "    <a class='download' href='" + app.url + "'>Download .apk</a>";
        }
    } else {
        outputHTML += "    <a class='download disabled' href='#appId" + app.id + "'>Not available</a>";
    }
    if (app.deps && app.deps.length > 0) {
        for (var i = 0, dep; dep = market.apps[i]; i++) {
            if ($.inArray(dep.id, app.deps) >= 0) {
                outputHTML += " <a href='#appId" + dep.id + "'>" + dep.title + "</a>";
            }
        }
    }
    outputHTML += "</small></p>";

    outputHTML += "</li>";

    return outputHTML;

}

////////////////////////////////////////////////////////////////////////////////
//  View
////////////////////////////////////////////////////////////////////////////////
$(document).ready(function() {

    var output = "";
    for (var i = 0, app; app = market.apps[i]; i++) {
        output += generateApp(app, market);
    }
    $("#appList").html(output);

});
