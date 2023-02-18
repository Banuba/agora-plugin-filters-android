function Effect() {
    var self = this;

    this.hintSolver = function() {
		now = (new Date()).getTime();

        if (now > self.removeHint) {
            Api.hideHint();
            self.faceActions = [];
        }
	};

    this.init = function() {
        Api.meshfxMsg("spawn", 2, 0, "!glfx_FACE");
        Api.meshfxMsg("spawn", 0, 0, "Face.bsm2");
        Api.meshfxMsg("spawn", 1, 0, "Policeman.bsm2");    

        Api.playSound("Policeman_sndfnt.ogg", true, 1);
        Api.showRecordButton();
        
        self.removeHint = (new Date()).getTime() + 5000;

        self.faceActions = [self.hintSolver];
    };

    this.restart = function() {
        Api.meshfxReset();
        self.init();
    };

    this.startSound = function () {
        Api.stopSound("Policeman_sndfnt.ogg");
		Api.playSound("Policeman_sndfnt.ogg", true, 1);
	};

	this.stopSound = function () {
		if(Api.getPlatform() == "ios") {
			Api.hideHint();
			Api.stopSound("Policeman_sndfnt.ogg");			
		};
	};

    this.faceActions = [];
    this.noFaceActions = [];

	this.videoRecordStartActions = [self.stopSound];
	this.videoRecordFinishActions = [];
	this.videoRecordDiscardActions = [self.startSound];
}

configure(new Effect());