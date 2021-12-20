function Effect() {
    var self = this;

    this.meshes = [
        { file: "Morph.bsm2", anims: [
            { a: "CINEMA_4D_Main", t: 1000 },
        ] },
        { file: "hat.bsm2", anims: [
            { a: "CINEMA_4D_Main", t: 1000 },
        ] },
    ];

    this.play = function() {
        now = (new Date()).getTime();
        if (now > self.removeHint) {
            Api.hideHint();
            self.faceActions = [];
        }
    }
/*
    this.play = function() {
        var now = (new Date()).getTime();
        for(var i = 0; i < self.meshes.length; i++) {
            if(now > self.meshes[i].endTime) {
                self.meshes[i].animIdx = (self.meshes[i].animIdx + 1)%self.meshes[i].anims.length;
                Api.meshfxMsg("animOnce", i, 0, self.meshes[i].anims[self.meshes[i].animIdx].a);
                self.meshes[i].endTime = now + self.meshes[i].anims[self.meshes[i].animIdx].t;
            }
        }
    };
*/
    this.init = function() {
        Api.meshfxMsg("spawn", 2, 0, "!glfx_FACE");
        Api.meshfxMsg("spawn", 0, 0, "Morph.bsm2");
        Api.meshfxMsg("spawn", 1, 0, "hat.bsm2");

        for(var i = 0; i < self.meshes.length; i++) {
            self.meshes[i].animIdx = -1;
            self.meshes[i].endTime = 0;
        };
        if(Api.getPlatform() == "ios") {
            Api.showHint("Voice changer");
        };
        
        self.removeHint = (new Date()).getTime() + 5000;
        // Api.showHint("Open mouth");
        // Api.playVideo("frx",true,1);
        // Api.playSound("music.ogg",true,1);
        Api.showRecordButton();
        self.faceActions = [this.play];
    };

    this.removeSound = function() {
        if(Api.getPlatform() == "ios") {
            Api.hideHint();
            // Api.stopSound("music.ogg");
        };
    };

    this.restart = function() {
        //Api.meshfxReset();
        // Api.stopVideo("frx");
        Api.stopSound("music.ogg");
        self.init();
    };

    this.faceActions = [];
    this.noFaceActions = [];

    this.videoRecordStartActions = [this.removeSound];
    this.videoRecordFinishActions = [];
    this.videoRecordDiscardActions = [this.restart];
}

configure(new Effect());