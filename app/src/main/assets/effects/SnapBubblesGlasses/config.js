var settings = {
    effectName: "SnapBubblesGlasses"
}

var spendTime = 0;
var analytic = {
    spendTimeSec: 0
}

function sendAnalyticsData() {
    var _analytic;
    analytic.spendTimeSec = Math.round(spendTime / 1000);
    _analytic = {
        "Event Name": "Effects Stats",
        "Effect Name": settings.effectName,
        "Spend Time": String(analytic.spendTimeSec)
    };
    Api.print("sended analytic: " + JSON.stringify(_analytic));
    Api.effectEvent("analytic", _analytic);
}

function Effect() {
    var self = this;

    this.init = function() {
        Api.meshfxMsg("spawn", 2, 0, "!glfx_FACE");
        Api.meshfxMsg("spawn", 0, 0, "glasses.bsm2");
        Api.meshfxMsg("spawn", 1, 0, "morph.bsm2");
        Api.meshfxMsg("spawn", 3, 0, "quad.bsm2");
        Api.playVideo("foreground", true, 1);        
        Api.playVideo("frx", true, 1);
        Api.playSound("music.ogg", true, 1);
        // Api.showHint("Open mouth");

        Api.showRecordButton();
    };

    this.restart = function() {
        Api.meshfxReset();
        Api.stopSound("music.ogg");
        self.init();
    };

    this.timeUpdate = function () {
        if (self.lastTime === undefined) self.lastTime = (new Date()).getTime();

        var now = (new Date()).getTime();
        self.delta = now - self.lastTime;
        if (self.delta < 3000) { // dont count spend time if application is minimized
            spendTime += self.delta;
        }
        self.lastTime = now;
    };

    this.faceActions = [this.timeUpdate];
    this.noFaceActions = [this.timeUpdate];

    this.videoRecordStartActions = [];
    this.videoRecordFinishActions = [];
    this.videoRecordDiscardActions = [this.restart];
}

function onStop() {
    try {
        sendAnalyticsData();
    } catch (err) {
        Api.print(err);
    }
}

function onFinish() {
    try {
        sendAnalyticsData();
    } catch (err) {
        Api.print(err);
    }
}

configure(new Effect());