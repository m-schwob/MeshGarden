import 'dart:async';
import 'package:flutter/material.dart';

class LoadingScreen extends StatefulWidget {
  @override
  _LoadingScreenState createState() => _LoadingScreenState();
}
class _LoadingScreenState extends State<LoadingScreen> {
  @override
  void initState() {
    super.initState();
    Timer(Duration(seconds: 10),()=>print("")
            );
  }
  @override
  Widget build(BuildContext context) {
    return Container(
      padding: const EdgeInsets.all(50),
      child: Image(
        image: AssetImage(
        'assets/images/logo.png',
    ),
        // child:FlutterLogo(size:MediaQuery.of(context).size.height)
    ),);
  }
}

// class LoadingScreen extends StatelessWidget {
//   const LoadingScreen({Key? key}) : super(key: key);
//
//
//   @override
//   Widget build(BuildContext context) {
//     return Container(
//         color: Colors.white,
//         child:FlutterLogo(size:MediaQuery.of(context).size.height)
//     );
//   }
//
// class MyHomePage extends StatefulWidget {
//   @override
//   _MyHomePageState createState() => _MyHomePageState();
// }
// class _MyHomePageState extends State<MyHomePage> {
//   @override
//   void initState() {
//     super.initState();
//     Timer(Duration(seconds: 3),
//             ()=>Navigator.pushReplacement(context,
//             MaterialPageRoute(builder:
//                 (context) =>
//                 SecondScreen()
//             )
//         )
//     );
//   }
//   @override
//   Widget build(BuildContext context) {
//     return Container(
//         color: Colors.white,
//         child:FlutterLogo(size:MediaQuery.of(context).size.height)
//     );
//   }
// }
//
