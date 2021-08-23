function Effect() {
    var self = this;

    this.meshes = [
        { file: "Plane.bsm2", anims: [
            { a: "Take 001", t: 5000 },
        ] },
        { file: "face_01.bsm2", anims: [
            { a: "static", t: 0 },
        ] },
        { file: "hat.bsm2", anims: [
            { a: "static", t: 0 },
        ] },
    ];

    this.play = function() {
        var now = (new Date()).getTime();
        for(var i = 0; i < self.meshes.length; i++) {
            if(now > self.meshes[i].endTime) {
                self.meshes[i].animIdx = (self.meshes[i].animIdx + 1)%self.meshes[i].anims.length;
                Api.meshfxMsg("animOnce", i, 0, self.meshes[i].anims[self.meshes[i].animIdx].a);
                self.meshes[i].endTime = now + self.meshes[i].anims[self.meshes[i].animIdx].t;
            }
        }

        // if(isMouthOpen(world.landmarks, world.latents)) {
        //  Api.hideHint();
        // }
    };

    this.init = function() {
        Api.meshfxMsg("spawn", 2, 0, "!glfx_FACE");

        Api.meshfxMsg("spawn", 0, 0, "face_01.bsm2");
        // Api.meshfxMsg("animOnce", 0, 0, "static");

        Api.meshfxMsg("spawn", 1, 0, "hat.bsm2");
        // Api.meshfxMsg("animOnce", 1, 0, "static");

        Api.meshfxMsg("spawn", 3, 0, "Plane.bsm2");
        // Api.meshfxMsg("animOnce", 0, 0, "Take 001");
        Api.meshfxMsg("dynImass", 3, 0, "joint1");
        Api.meshfxMsg("dynImass", 3, 10, "joint2");
        Api.meshfxMsg("dynImass", 3, 10, "joint3");
        Api.meshfxMsg("dynImass", 3, 11, "joint4");
        
        
        Api.meshfxMsg("dynDamping", 3, 95);
        Api.meshfxMsg("dynGravity", 3, 0, "0 300 0");

        
        self.faceActions = [self.play];
        // Enable background audio playback
        Api.playVideo("frx",true,1);
        Api.playSound("music_L_Ch.ogg", true, 1);
        // Api.showHint("Open mouth");
        Api.showHint("");
        

        Api.showRecordButton();
    };

    this.restart = function() {
        Api.meshfxReset();
        Api.stopSound("music_L_Ch.ogg");
        self.init();
    };

    this.faceActions = [];
    this.noFaceActions = [];

    this.videoRecordStartActions = [];
    this.videoRecordFinishActions = [];
    this.videoRecordDiscardActions = [this.restart];
}

configure(new Effect());
