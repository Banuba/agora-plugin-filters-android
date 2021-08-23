function Effect() {
    var self = this;

    this.init = function() {
        Api.meshfxMsg("spawn", 4, 0, "!glfx_FACE");
        Api.meshfxMsg("spawn", 0, 0, "BeautyFaceSP_Optimased.bsm2");
        Api.meshfxMsg("spawn", 2, 0, "TitanicMan.bsm2");
        Api.meshfxMsg("spawn", 3, 0, "tri.bsm2");
        Api.playVideo("foreground", true, 1);
        Api.playSound("music.ogg", true, 1);

        Api.showRecordButton();
    };

    this.restart = function() {
        Api.meshfxReset();
        Api.stopSound("music.ogg");
        self.init();
    };

    this.faceActions = [];
    this.noFaceActions = [];

    this.videoRecordStartActions = [];
    this.videoRecordFinishActions = [];
    this.videoRecordDiscardActions = [this.restart];
}

configure(new Effect());