function Effect() {
    var self = this;

    this.init = function() {
        Api.meshfxMsg("spawn", 2, 0, "!glfx_FACE");

        Api.meshfxMsg("spawn", 0, 0, "botan.bsm2");

        Api.meshfxMsg("spawn", 1, 0, "morph.bsm2");
       
        if(Api.getPlatform() == "iOS"){
            Api.showHint("Voice changer");
        };
        
        self.time = (new Date()).getTime() + 5000;
        self.faceActions = [self.hintTimer];
        
        Api.showRecordButton();
    };

    this.hintTimer = function(){
    	var now = (new Date()).getTime();
        if (now > self.time){
            Api.hideHint();
            self.faceActions = [];
        };
    };

    this.restart = function() {
        Api.meshfxReset();
        self.init();
    };

    this.faceActions = [];
    this.noFaceActions = [];

    this.videoRecordStartActions = [this.restart];
    this.videoRecordFinishActions = [];
    this.videoRecordDiscardActions = [this.restart];
}

configure(new Effect());