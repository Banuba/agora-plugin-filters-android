function Effect()
{
    var self = this;

    this.init = function() {
        Api.meshfxMsg("spawn", 1, 0, "tri1.bsm2");
        Api.meshfxMsg("tex", 1, 0, "LUT_5.png");
        Api.meshfxMsg("spawn", 15, 0, "quad_tex5.bsm2");
        spawnDate(1);
    };

    this.faceActions = [];
    this.noFaceActions = [];

    this.videoRecordStartActions = [];
    this.videoRecordFinishActions = [];
    this.videoRecordDiscardActions = [];
}

configure(new Effect());

function calcUVShift1(num) {
    var result = [];
    switch (num) {
        case 0:
            result[0] = 0.;
            result[1] = 0.;
            break;
        case 1:
            result[0] = 0.25;
            result[1] = 0.;
            break;
        case 2:
            result[0] = 0.5;
            result[1] = 0.;
            break;
        case 3:
            result[0] = 0.75;
            result[1] = 0.;
            break;
        case 4:
            result[0] = 0.;
            result[1] = -1./3.;
            break;
        case 5:
            result[0] = 0.25;
            result[1] = -1./3.;
            break;
        case 6:
            result[0] = 0.5;
            result[1] = -1./3.;
            break;
        case 7:
            result[0] = 0.75;
            result[1] = -1./3.;
            break;
        case 8:
            result[0] = 0.;
            result[1] = -2./3.;
            break;
        case 9:
            result[0] = 0.25;
            result[1] = -2./3.;
            break;
        default:
            break;
    }
    return result;
}

function calcUVShift2(dateType,num) {
    var result = [];
    var xStartMonth = 0.05;
    var xShiftMonth = 0.46;

    var yShiftMonth = 1./6.;
    var yShiftYear = 0.0725;
    if (dateType == 'month'){
        switch (num) {
            case 1:
                result[0] = xStartMonth;
                result[1] = 0.;
                break;
            case 2:
                result[0] = xStartMonth;
                result[1] = 0. - yShiftMonth;
                break;
            case 3:
                result[0] = xStartMonth;
                result[1] = 0. - yShiftMonth * 2;
                break;
            case 4:
                result[0] = xStartMonth;
                result[1] = 0. - yShiftMonth * 3;
                break;
            case 5:
                result[0] = xStartMonth;
                result[1] = 0. - yShiftMonth * 4;
                break;
            case 6:
                result[0] = xStartMonth;
                result[1] = 0. - yShiftMonth * 5;
                break;
            case 7:
                result[0] = xStartMonth + xShiftMonth;
                result[1] = 0;
                break;
            case 8:
                result[0] = xStartMonth + xShiftMonth;
                result[1] = 0. - yShiftMonth;
                break;
            case 9:
                result[0] = xStartMonth + xShiftMonth;
                result[1] = 0. - yShiftMonth * 2;
                break;
            case 10:
                result[0] = xStartMonth + xShiftMonth;
                result[1] = 0. - yShiftMonth * 3;
                break;
            case 11:
                result[0] = xStartMonth + xShiftMonth;
                result[1] = 0. - yShiftMonth * 4;
                break;
            case 12:
                result[0] = xStartMonth + xShiftMonth;
                result[1] = 0. - yShiftMonth * 5;
                break;
            default:
                break;
        }
        return result;
    } else if (dateType == "year") {
        result[0] = 0;
        result[1] = 0. + (num - 1) * yShiftYear;
        return result;
    } else {
        Api.print("calcUVShift2() - passed dateType param should be 'month' or 'year'");
    }
}

function spawnDate(type) {

    var xPosDigit = 0.;
    var yPosDigit = -0.25;

    var scaleDigit = .75;
    var angleDigit = 90;

    var xPosScript = 0.2;
    var yPosScript = 0.3;

    var scaleScript = 1.;
    var angleScript = -90;

    var day = (new Date()).getDate();
    var month = (new Date()).getMonth();
    var year = (new Date()).getFullYear();

    var yearDigit3 = parseInt(year.toString().substring(2,3));
    var yearDigit4 = parseInt(year.toString().substring(3,4));

    if (type == 1) {

        Api.meshfxMsg("spawn", 2, 0, "geo_digital.bsm2");

        if(month.toString().length == 2) {
            var monthDigit1 = parseInt(month.toString().substring(0,1));
            var monthDigit2 = parseInt(month.toString().substring(1,2)) + 1;
        } else {
            var monthDigit1 = 0;
            var monthDigit2 = parseInt(month.toString().substring(0,1)) + 1;
        }
        var shiftMonth1 = calcUVShift1(monthDigit1);
        var shiftMonth2 = calcUVShift1(monthDigit2);

        if(day.toString().length == 2) {
            var dayDigit1 = parseInt(day.toString().substring(0,1));
            var dayDigit2 = parseInt(day.toString().substring(1,2));
        } else {
            var dayDigit1 = 0;
            var dayDigit2 = parseInt(day.toString().substring(0,1));
        }
        var shiftDay1 = calcUVShift1(dayDigit1);
        var shiftDay2 = calcUVShift1(dayDigit2);

        var shiftYear1 = calcUVShift1(yearDigit3);
        var shiftYear2 = calcUVShift1(yearDigit4);

            // Send pos and scale edit
        Api.meshfxMsg("shaderVec4", 0, 2, xPosDigit + " " + yPosDigit + " " + scaleDigit + " " + angleDigit);
            
            // Send time data
        Api.meshfxMsg("shaderVec4", 0, 3, shiftMonth1[0] + " " + shiftMonth1[1] + " " + shiftMonth2[0] + " " + shiftMonth2[1]); // month
        Api.meshfxMsg("shaderVec4", 0, 4, shiftDay1[0] + " " + shiftDay1[1] + " " + shiftDay2[0] + " " + shiftDay2[1]); // day
        Api.meshfxMsg("shaderVec4", 0, 5, shiftYear1[0] + " " + shiftYear1[1] + " " + shiftYear2[0] + " " + shiftYear2[1]); // year

    } else if (type == 2) {
        var shiftMonth1 = calcUVShift2('month',month + 1);

        var shiftYear1 = calcUVShift2('year',-yearDigit3+1);
        var shiftYear2 = calcUVShift2('year',-yearDigit4+1);

            // Send pos and scale edit
        Api.meshfxMsg("shaderVec4", 0, 2, xPosScript + " " + yPosScript + " " + scaleScript + " " + angleScript);
            // Send time data
        Api.meshfxMsg("shaderVec4", 0, 3, shiftMonth1[0] + " " + shiftMonth1[1] + " 0 0"); // month
        Api.meshfxMsg("shaderVec4", 0, 4, shiftYear1[0] + " " + shiftYear1[1] + " " + shiftYear2[0] + " " + shiftYear2[1]);

        // Api.meshfxMsg("spawn", 2, 0, "geo_script.bsm2");
    }
}