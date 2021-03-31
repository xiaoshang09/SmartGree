
// 基于准备好的dom，初始化echarts实例
//var myChart = echarts.init(document.getElementById('main'));
var myChart = echarts.init(document.getElementById('main'), 'dark');
//var myChart = echarts.init(document.getElementById('main'), 'styleDark');
// var myChart = echarts.init(document.getElementById('main'), 'myDark');

var _backgroundColor = 'rgba(45, 45, 45, 0.9)'
var lineColor= ['#516b91','#59c4e6','#a5e7f0','#fcce10','#edafda','#93b7e3','#a5e7f0','#cbb0e3','#22c3aa','#4ea397' ]

console.log(myChart);

function setSize(size){
    var main = document.getElementById('main');
    main.style.width = size["width"] + "px";
    main.style.height = size["height"] + "px";
    myChart.resize();
}

function init(){
    var option = {
        backgroundColor : _backgroundColor,
        title: {
            text: "空数据",
            x: "center"
        },
        // tooltip: {
        //     trigger: "item",
        //     // formatter: "{a} <br/>{b} : {c}"
        // },
        legend: {
            x: 'left',
        },
        xAxis: [
            {
                type: "category",
                name: "时间轴",
                splitLine: {show: false},
                data: ["一", "二", "三", "四", "五", "六", "七",]
            }
        ],
        yAxis: [
            {
                type: "log",
                name: "数据"
            }
        ],
        calculable: true,
        series: [
            {
                name: "无产品",
                type: "bar",
                // data: [0, 2, 3, 4, 5, 6, 7, 9, 5]

            }
        ]
    };

    myChart.setOption(option);
}

function addLineData(seriesList, lineIndex, lineType,  lineName, lineData, stackName , tColor, toFixed){

    seriesList.push({
                        type: lineType,
                        name: lineName,
                        //        stack: 'chart',3.注意stack---数据堆叠，同个类目轴上系列配置相同的stack值后，
                        //后一个系列的值会在前一个系列的值上相加。 所以最后的解决方法是，，stack去掉，，或者stack给不同的值。。
                        stack: stackName,
                        xAxisIndex: lineIndex,
                        yAxisIndex: lineIndex,

                        label: {
                            normal: {
                                position: 'right',
                                show: true,
                            },
                        },

                        //柱子分组间隔0
                        barGap:0 ,
                        //有效区分0和1
                        barMinHeight: 1,
                        //显示数据点 auto如果有足够空间则显示数据点，否则随主轴标签间隔隐藏策略。 false随主轴标签间隔隐藏策略。
                        showSymbol: 'auto',
                        showAllSymbol: 'auto',
                        //抽样
                        sampling: 'average',

                        // markPoint: { //最大最小值标记
                        //     data: [{
                        //             type: 'max',
                        //             name: '最大值',
                        //             // 先让markLine精确到3，默认为2
                        //             precision: 3,
                        //             label: {
                        //                 formatter: function(value) {
                        //                     // 确保你的Number.toFixed(2)是满足要求的
                        //                     return value.value.toFixed(2);
                        //                 }
                        //             }
                        //         },
                        //         {
                        //             type: 'min',
                        //             name: '最小值',
                        //             // 先让markLine精确到3，默认为2
                        //             precision: 3,
                        //             label: {
                        //                 formatter: function(value) {
                        //                     // 确保你的Number.toFixed(2)是满足要求的
                        //                     return value.value.toFixed(2);
                        //                 }
                        //             }
                        //         }]
                        // },
                        data:lineData,
                    });

}


//测试用数据
var _jsObj =
        {
    "data": {
        "xData": ["11-01", "11-02", "11-03", "11-04", "11-05", "11-06", "11-07", "11-08", "11-09", "11-10", "11-11", "11-12", "11-13", "11-14", "11-15", "11-16", "11-17", "11-18", "11-19", "11-20", "11-21", "11-22", "11-23", "11-24", "11-25", "11-26", "11-27", "11-28", "11-29", "11-30"],
        "yData": {
            "AAAAA": [
                ["11-09", "AAAAA", 587, 559, 28, 0.5507403706249437, 29.42105263157895],
                ["11-10", "AAAAA", 1536, 1441, 95, 0.8247769980719595, 43.666666666666664],
                ["11-16", "AAAAA", 0, 0, 0, 0, 0],
                ["11-28", "AAAAA", 1303, 1208, 95, 0.9264527040979137, 52.52173913043478],
                ["11-29", "AAAAA", 2236, 2088, 148, 0.7716578531723756, 40.15384615384615],
                ["11-30", "AAAAA", 0, 0, 0, 0, 0]
            ],
            "BBBBB": [
                ["11-03", "BBBBB", 0, 0, 0, 0, 0],
                ["11-09", "BBBBB", 0, 0, 0, 0, 0],
                ["11-23", "BBBBB", 0, 0, 0, 0, 0],
                ["11-28", "BBBBB", 0, 0, 0, 0, 0],
                ["11-29", "BBBBB", 2030, 1879, 151, 0.45672650893529254, 25.053333333333335],
                ["11-30", "BBBBB", 0, 0, 0, 0, 0]
            ],
            "CCCCCC": [
                ["11-10", "CCCCCC", 160, 142, 18, 0.3944444523917304, 15.777777777777779],
                ["11-22", "CCCCCC", 1665, 1570, 95, 0.8391835496232316, 42.432432432432435],
                ["11-23", "CCCCCC", 42, 41, 1, 0.244047611951828, 10.25],
                ["11-28", "CCCCCC", 1188, 1118, 70, 0.7537134667237599, 37.266666666666666],
                ["11-29", "CCCCCC", 1649, 1535, 114, 0.6712084970575698, 32.659574468085104],
                ["11-30", "CCCCCC", 630, 570, 60, 0.6168831072070382, 25.90909090909091]
            ],
            "OthersData": [
                ["11-01", "", "", "", ""],
                ["11-02", "", "", "", ""],
                ["11-03", 0, 1, 360, ""],
                ["11-04", "", "", "", ""],
                ["11-05", "", "", "", ""],
                ["11-06", "", "", "", ""],
                ["11-07", "", "", "", ""],
                ["11-08", "", "", "", ""],
                ["11-09", 0, 1, 3120, ""],
                ["11-10", 0, 15, 4920, ""],
                ["11-11", "", "", "", ""],
                ["11-12", "", "", "", ""],
                ["11-13", "", "", "", ""],
                ["11-14", "", "", "", ""],
                ["11-15", "", "", "", ""],
                ["11-16", 0, 0, 360, ""],
                ["11-17", "", "", "", ""],
                ["11-18", "", "", "", ""],
                ["11-19", "", "", "", ""],
                ["11-20", "", "", "", ""],
                ["11-21", "", "", "", ""],
                ["11-22", 0, 1, 4320, ""],
                ["11-23", 0, 1, 600, ""],
                ["11-24", "", "", "", ""],
                ["11-25", "", "", "", ""],
                ["11-26", "", "", "", ""],
                ["11-27", "", "", "", ""],
                ["11-28", 0, 9, 6960, ""],
                ["11-29", 0, 48, 20760, ""],
                ["11-30", 0, 4, 3240, ""]
            ]
        }
    },
    "dataSum": {
        "xData": [],
        "yData": {
            "AAAAA": [
                ["2019-11", "AAAAA", 5662, 5296, 366, 0.5122713446617126, 27.62721824645996]
            ],
            "BBBBB": [
                ["2019-11", "BBBBB", 2030, 1879, 151, 0.07612108439207077, 4.17555570602417]
            ],
            "CCCCCC": [
                ["2019-11", "CCCCCC", 5334, 4976, 358, 0.5865800976753235, 27.382591247558594]
            ],
            "OthersData": [
                ["2019-11", 0, 80, 1488, ""]
            ]
        }
    },
    "tableName": "月表:2019-11"
}

function setData(jsObj) {
    jsObj = _jsObj;

    var gridsList  = [];
    var xAxisList  = [];
    var yAxisList  = [];
    var titleList  = [];
    var seriesList = [];
    //
    var xData     = jsObj['data']['xData'];
    var inData    = jsObj['data']['yData'];
    var inDataSum = jsObj['dataSum']['yData'];
    var titleNameList = jsObj['tableName'];
    //
    var pruductList = Object.keys(inDataSum);
    if (pruductList.length === 0) {
        console.log("data null !");
        myChart.clear();
        init();
    } else {
        console.log("pruductList:" + pruductList) ;
    }

    var pieTitles= ["产量统计"] // "耗电量统计"

    //统计图
    for(let j = 0; j < 1; j++){

        let tdata=inDataSum;
        let lineName =  pieTitles[j];
        let pieData = [];

        //======统计图  data=====================
        for(let idx=0;idx<pruductList.length;idx++){
            if (pruductList[idx] === 'OthersData') {
                // TODO 特殊数据
                continue;
            }else{
                // var  pdtTotal = tdata[pruductList[idx]].map(function(item) {
                //     return [item[0], item[2]];
                // });

                let pdtOK  = tdata[pruductList[idx]].map(function(item) {
                    let val = item[3];
                    if(!val){
                        val=0;
                    }
                    return [pruductList[idx] , val];
                });

                let pdtNG  = tdata[pruductList[idx]].map(function(item) {
                    let val = item[4];
                    if(!val){
                        val=0;
                    }
                    return [pruductList[idx] , val];
                });

                // console.log("pdtOK[0][1]=="+pdtOK[0][1]);
                pieData.push({  name: pdtOK[0][0]+"合格", value: pdtOK[0][1] });
                pieData.push({  name: pdtNG[0][0]+"不良", value: pdtNG[0][1] });
            }
        }

        //======统计图  pos=====================
        let startSpace = 9;
        let space = 5;
        let titleH = 3;
        let h=45;
        let _top=0+titleH+startSpace;
        let centerX= 50;
        let centerY= space+titleH+h/2;

        titleList.push({
                           text: lineName,
                           left: '80%',
                           top: (_top-titleH-1) + '%',
                           textAlign: 'center',
                       });

        seriesList.push({
                            name: lineName,
                            type: 'pie',
                            radius: [0, (h-titleH)+'%'],
                            center: [ centerX+'%', centerY + '%'],
                            data: pieData,
                            //是否在数据和为0（一般情况下所有数据为0） 的时候不显示扇区。
                            stillShowZeroSum : true,
                            tooltip: {
                                trigger: "item",
                                formatter: "{a} <br/>{b} : {c}  ({d}%)"
                                // formatter: "{a} <br/>{b} : {c}",
                            },

                            // label: {
                            //     normal: {
                            //         show: true,
                            //         // position:'inside'
                            //     }
                            // },

                            // startAngle:180,//起始角度，支持范围[0, 360]。
                            minAngle: 3, //最小的扇区角度（0 ~ 360），用于防止某个值过小导致扇区太小影响交互
                            itemStyle:{
                                normal:{
                                    label:{
                                        // //通过设置边框宽度，并给边框设置背景同色来表示扇形直接的间隔距离
                                        // borderWidth: 1,
                                        // borderColor: '#fff8',
                                        show: true,
                                        formatter: '{b} : {c} ({d}%)' ,
                                    } ,
                                } ,
                            },
                        });
    }

    //折线图
    for(let i = 0; i < 1; i++) {

        //======折线图 pos=====================
        let startSpace = 9;
        let space = 5;
        let titleH = 3;
        let h=34;
        let _top=52+titleH+startSpace;

        titleList.push({
                           text: '节拍',
                           left: '80%',
                           top: (_top-titleH-2) + '%',
                           textAlign: 'center',
                       });
        gridsList.push({
                           show: true,
                           //不显示Y最上面那条?
                           borderWidth: 0,
                           containLabel: true,//包含标签//就是显示Y标签
                           left: '2%',
                           top: _top+'%',
                           // width: _width+'%',
                           height: h+'%',
                       });
        xAxisList.push({
                           type: 'category',//category 离散型数据,value 连续数据
                           show: true,
                           boundaryGap: true, //留点边距
                           gridIndex: i,
                           //                           splitNumber: 20, //11个刻度线，也就是10等分//无效
                           // axisLabel: {
                           // interval:0,//强制显示所有标签
                           // rotate:40
                           // },
                           // data:[00,01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24],
                           // axisTick:{interval:0,},//强制显示所有标签
                           scale:true,// 智适应 //是否是脱离 0 值比例
                           data: xData,
                       });
        yAxisList.push({
                           show: true,
                           type: 'log',
                           // min: 0.1,
                           // logBase:2,

                           //类目轴直接设置百分比
                           boundaryGap: ['5%', '5%'],
                           // left: 100,
                           gridIndex: i,
                           splitLine: {
                               show: false, //不显示横格
                           },
                           scale:true,// 智适应//有最大 最小值无法智适应 //是否是脱离 0 值比例
                           //   splitNumber: 20, //11个刻度线，也就是10等分//仅非类目有效
                       });

    }

    for(let idx = 0; idx < pruductList.length; idx++) {
        //======条形图 填充数据=====================
        let tdata=inData;
        let lineName = pruductList[idx];


        if (pruductList[idx] === 'OthersData') {
            //TODO 特殊数据
            continue;
        }else{
            let  pdtTaktTime = tdata[pruductList[idx]].map(function(item) {

                let val = item[6];
                if(!val){
                    val=0;
                }
                if(parseInt(val)!==val){
                    val = parseFloat( val ).toFixed(2);
                }
                return [item[0], val];
                // return val;
            });

            addLineData(seriesList, 0, 'line', lineName+":节拍", pdtTaktTime );
        }
    }

    option = {
        backgroundColor :_backgroundColor ,
        // shadowColor: 'rgba(0, 0, 0, 0.3)',
        // shadowBlur: 2,
        legend: {
            show:true,
            x:'left',
            y: 'top',
            // data:legendData,
        },
        title: titleList,
        grid: gridsList,
        xAxis: xAxisList,
        yAxis: yAxisList,
        series: seriesList,

        tooltip:
        {
            trigger: 'axis',
            axisPointer: {            // 坐标轴指示器，坐标轴触发有效
                type: 'shadow'        // 默认为直线，可选为：'line' | 'shadow'
            }
        },

        toolbox: {
            show: true,
            top: '58%',
            left: 'left',
            feature: {
                mark: {
                    show: true
                },
                dataZoom:[{
                        // xAxisIndex: 0,
                        // yAxisIndex: 'none',
                        yAxisIndex: false,
                        start: -5,
                        end: +105,
                    },{
                        // xAxisIndex:'none' ,
                        xAxisIndex: false,
                        start: -5,
                        end: +105,
                    },
                ],
                dataView: {
                    show: true,
                    readOnly: true,
                },
                magicType: {
                    show: true,
                    type: ['line', 'bar'],
                },
                restore: {
                    show: true,
                }
            }
        }
    };

    // myChart.clear();
    myChart.setOption(option, true);
    myChart.resize();
};


window.onresize = myChart.resize ;
init()

