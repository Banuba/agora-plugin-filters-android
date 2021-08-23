function Effect() {
    var self = this;

    this.meshes = [
        { file: "Face_05.bsm2", anims: [
            { a: "static", t: 0 },
        ] },
        { file: "hat.bsm2", anims: [
            { a: "Take 001", t: 5000 },
        ] },
    ];

    this.play = function() {
        now = (new Date()).getTime();

        if (now > self.time){
            Api.hideHint();
        };
        if(isMouthOpen(world.landmarks, world.latents)) {
            Api.hideHint();
        };
    };

    this.init = function() {
        Api.meshfxMsg("spawn", 2, 0, "!glfx_FACE");

        Api.meshfxMsg("spawn", 0, 0, "Face_05.bsm2");
        // Api.meshfxMsg("animOnce", 0, 0, "static");

        Api.meshfxMsg("spawn", 1, 0, "hat.bsm2");
        // Api.meshfxMsg("animOnce", 1, 0, "Take 001");

        Api.showHint("")
        self.time = (new Date()).getTime() + 5000;
        self.faceActions = [self.play];
        // Api.showHint("Open mouth");
        Api.playVideo("backgroundSeparation", true, 1);
        Api.playSound("cap_sndfnt.ogg",true,1);
        Api.showRecordButton();
    };

    this.restart = function() {
        Api.meshfxReset();
        // Api.stopVideo("frx");
        // Api.stopSound("sfx.aac");
        self.init();
    };

    this.stopSound = function () {
        if(Api.getPlatform() == "ios") {
            Api.hideHint();
            Api.stopSound("cap_sndfnt.ogg");                  
        };

    };

    this.faceActions = [];
    this.noFaceActions = [];

    this.videoRecordStartActions = [self.stopSound];
    this.videoRecordFinishActions = [];
    this.videoRecordDiscardActions = [this.restart];
}

configure(new Effect());
