var settings = {
    effectName: "MorphTeenegers"
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

    var timer = new Date().getTime();
    var delay = 3000;

    this.hideHint = function() {
        var now = new Date().getTime();

        if (now >= timer + delay) {
            Api.hideHint();
            var index = self.faceActions.indexOf(self.hideHint);
            self.faceActions.splice(index, 1);
            }
    };

    this.init = function() {
        Api.meshfxMsg("spawn", 5, 0, "!glfx_FACE");
        Api.meshfxMsg("spawn", 0, 0, "Cut.bsm2");
        Api.meshfxMsg("spawn", 1, 0, "Glasses.bsm2");
        Api.meshfxMsg("spawn", 2, 0, "Hair.bsm2");
        Api.meshfxMsg("spawn", 3, 0, "Hat.bsm2");
        Api.meshfxMsg("spawn", 4, 0, "Morph.bsm2");
        // Api.playSound("music.ogg", true, 1);
        
        Api.showHint("");
        self.faceActions.push(self.hideHint);

        Api.showRecordButton();
    };

    this.restart = function() {
        Api.meshfxReset();
        // Api.stopSound("music.ogg");
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
