
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
              Expanded(
                flex: 1,
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  crossAxisAlignment: CrossAxisAlignment.center,
                  children: [
                    Row(
                      mainAxisAlignment: MainAxisAlignment.end,
                      crossAxisAlignment: CrossAxisAlignment.center,
                      children: [
                        IconButton(
                          // padding: EdgeInsets.R,
                          iconSize: 30,
                          onPressed: () {
                            _openGraph(context);
                          },
                          icon: FaIcon(FontAwesomeIcons.chartLine,
                              color: Colors.blueAccent),
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
                      // ?.toStringAsPrecision(2)
                    Text('${value?.toStringAsPrecision(3)}$unit',
                        style: kHeadline.copyWith(color: Colors.white)),
                    Text(DateFormat('MMM d, h:mm a').format(date_time!),
                        style: kSensorText.copyWith(color: Colors.grey)),
                  ],
                ),
              ),
            ],
          ),
        ));
  }

  Future _openGraph(BuildContext context) => showDialog(
        context: context,
        builder: (context) => AlertDialog(
          contentPadding: EdgeInsets.zero,
          backgroundColor: kMainBG,
          scrollable: true,
          content: Card(
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
                                        // autoScrollingMode:
                                        //     AutoScrollingMode.start,
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
                    ),
                  ],
                ),
              )),
        ),
      );
}


