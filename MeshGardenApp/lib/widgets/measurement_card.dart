
import 'package:flutter/material.dart';
import 'package:font_awesome_flutter/font_awesome_flutter.dart';
import 'package:iot_firestore_flutter_app/const/custom_colors.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';
import 'package:intl/intl.dart';


import 'package:syncfusion_flutter_charts/charts.dart';

import '../model/MeasuresData.dart';

class MeasurementCard extends StatelessWidget {
  const MeasurementCard({
    Key? key,
    required this.value,
    required this.date_time,
    required this.name,
    required this.assetImage,
    required this.unit,
    // required this.data_map,
    required this.chart_data,
    // required this.trendData,
    // required this.linePoint
  }) : super(key: key);

  final num? value;
  final String? name;
  final String? unit;
  final DateTime? date_time;
  // final Map<DateTime, double> data_map;
  final List<MeasuresData> chart_data;

  // final List<double> trendData;
  // final Color linePoint;
  final String assetImage;

  @override
  Widget build(BuildContext context) {
//     chart_data.forEach((element) {
//       print(element.time);
//       print ("------");
//       print(element.value);
//       print("\n");
//     });
// print(chart_data);


    return Card(
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(18),
        ),
        shadowColor: Colors.white,
        elevation: 24,
        color: kMainBG,
        child: Container(
          width: MediaQuery.of(context).size.width * 0.8,
          height: 270,
          child: Row(
            children: [
              // IconButton(
              //   onPressed: () {
              //     _openGraph(context, chart);
              //   },
              //   icon: Icon(
              //     Icons.add_circle,
              //     color: Colors.blue,
              //   ),
              // ),
              Expanded(
                flex: 1,
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  crossAxisAlignment: CrossAxisAlignment.center,
                  children: [
                    // IconButton(
                    //   onPressed: () {
                    //     _openGraph(context, chart);
                    //   },
                    //   icon: Icon(
                    //     Icons.add_circle,
                    //     color: Colors.blue,
                    //   ),
                    // ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.end,
                      crossAxisAlignment: CrossAxisAlignment.center,
                      children: [
                        // Image(
                        //   width: 60,
                        //   image: AssetImage(
                        //     assetImage,
                        //   ),
                        // ),
                        IconButton(
                          // padding: EdgeInsets.R,
                          iconSize: 30,
                          onPressed: () {
                            _openGraph(context);
                          },
                          icon: FaIcon(FontAwesomeIcons.chartLine,
                              color: Colors.blueAccent),
                          // icon: Icon(
                          //   Icons.chart,
                          //   color: Colors.blue,
                          // ),
                        ),
                      ],
                    ),
                    Image(
                      width: 60,
                      image: AssetImage(
                        assetImage,
                      ),
                    ),
                    SizedBox(
                      height: 10,
                    ),
                    Text(name!, style: kBodyText.copyWith(color: Colors.white)),
                    SizedBox(
                      height: 10,
                    ),
                    Text('$value$unit',
                        style: kHeadline.copyWith(color: Colors.white)),
                    Text(DateFormat('MMM d, h:mm a').format(date_time!),
                        style: kSensorText.copyWith(color: Colors.grey)),
                  ],
                ),
              ),
              // Expanded(
              //   flex: 1,
              //   child: Container(
              //     padding:
              //     const EdgeInsets.symmetric(vertical: 30, horizontal: 8),
              //     child: Sparkline(
              //       data: trendData,
              //       lineWidth: 5.0,
              //       lineColor: Colors.white,
              //       averageLine: false,
              //       enableGridLines: true,
              //       gridLineLabelPrecision: 3,
              //       fillMode: FillMode.none,
              //       sharpCorners: false,
              //       pointsMode: PointsMode.last,
              //       pointSize: 20,
              //       pointColor: linePoint,
              //       useCubicSmoothing: true,
              //     ),
              //   ),
              // ),
              // Expanded(
              //   flex: 1,
              //   child: Container(
              //     padding:
              //     const EdgeInsets.symmetric(vertical: 30, horizontal: 8),
              //     child: Column(
              //         mainAxisSize: MainAxisSize.max,
              //         mainAxisAlignment: MainAxisAlignment.spaceBetween,
              //         crossAxisAlignment: CrossAxisAlignment.stretch,
              //         children: [
              //           Expanded(child: AnimatedLineChart(
              //             chart,
              //             key: UniqueKey(),
              //             gridColor: Colors.white,
              //             // textStyle: TextStyle(fontSize: 10, color: Colors.white),
              //             toolTipColor: Colors.white,
              //             toolTipColor: Colors.white,
              //           ),),
              //         ]
              //     ),
              //     // child: AnimatedLineChart(
              //     //   chart,
              //     //   key: UniqueKey(),
              //     //   gridColor: Colors.white,
              //     //   // textStyle: TextStyle(fontSize: 10, color: Colors.white),
              //     //   toolTipColor: Colors.white,
              //     // ),
              //   ),
              // ),
            ],
          ),
        ));
  }

  Future _openGraph(BuildContext context) => showDialog(
        context: context,
        // barrierDismissible: false,
        // builder: (context) => showGeneralDialog(
        //     context: context,
        //
        //
        // )
        builder: (context) => AlertDialog(
          contentPadding: EdgeInsets.zero,
          backgroundColor: kMainBG,
          scrollable: true,
          // title: Center( child: Text("Time and Value Chart")),
          content: Card(
              // shape: RoundedRectangleBorder(
              //   borderRadius: BorderRadius.circular(18),
              // ),
              shadowColor: Colors.white,
              elevation: 24,
              color: kMainBG,
              child: Container(
                width: MediaQuery.of(context).size.width * 0.8,
                height: 250,
                child: Row(
                  children: [
                    Expanded(
                      flex: 1,
                      child: Container(
                        child: Column(
                            mainAxisSize: MainAxisSize.max,
                            mainAxisAlignment: MainAxisAlignment.spaceBetween,
                            crossAxisAlignment: CrossAxisAlignment.stretch,
                            children: [
                              Expanded(
                                child: Container(
                                  child: SfCartesianChart(
                                      zoomPanBehavior: ZoomPanBehavior(
                                        enablePinching: true,
                                        zoomMode: ZoomMode.x,
                                        enablePanning: true,
                                      ),
                                      tooltipBehavior:
                                          TooltipBehavior(enable: true,color: Colors.redAccent,header: "Time : Value"),
                                      primaryXAxis: DateTimeAxis(
                                        autoScrollingMode:
                                            AutoScrollingMode.start,
                                        dateFormat: DateFormat('MMM d, h:mm a'),
                                      ),
                                      series: <ChartSeries>[
                                        // Renders line chart
                                        SplineAreaSeries<MeasuresData, DateTime>(
                                          markerSettings:  MarkerSettings(
                                              isVisible: true
                                          ),
                                            dataSource: chart_data,
                                            xValueMapper:
                                                (MeasuresData data, _) =>
                                                    data.time,
                                            yValueMapper:
                                                (MeasuresData data, _) =>
                                                    data.value,
                                          // color: Color.fromRGBO(8, 142, 255, 1),
                                        ),
                                      ]
                                  ),
                                ),
                              ),
                            ]),
                      ),
                      // child: Container(
                      //   padding: const EdgeInsets.symmetric(
                      //       vertical: 30, horizontal: 8),
                      //   child: AnimatedLineChart(
                      //     chart,
                      //     key: UniqueKey(),
                      //     gridColor: Colors.white,
                      //     // textStyle: TextStyle(fontSize: 10, color: Colors.white),
                      //     toolTipColor: Colors.white,
                      //   ),
                      // ),
                    ),
                  ],
                ),
              )),
        ),
      );
}


