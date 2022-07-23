import 'package:iot_firestore_flutter_app/route/routing_constants.dart';
import 'package:iot_firestore_flutter_app/screens/signin_screen.dart';
import 'package:iot_firestore_flutter_app/screens/signup_screen.dart';
import 'package:iot_firestore_flutter_app/screens/splash_screen.dart';
import 'package:iot_firestore_flutter_app/screens/undefined_screen.dart';
import 'package:iot_firestore_flutter_app/screens/nodes_screen.dart';
import 'package:iot_firestore_flutter_app/screens/loading_screen.dart';
import 'package:flutter/material.dart';

Route<dynamic> generateRoute(RouteSettings settings) {
  switch (settings.name) {
    case SplashScreenRoute:
      return MaterialPageRoute(builder: (context) => SplashScreen());

    case SignInScreenRoute:
      return MaterialPageRoute(builder: (context) => SignInScreen());

    case SignUpScreenRoute:
      return MaterialPageRoute(builder: (context) => SignUpScreen());


    case NodesScreenRoute:
      return MaterialPageRoute(builder: (context) => NodesScreen());

    case LoadingScreenRoute:
      return MaterialPageRoute(builder: (context) => LoadingScreen());


    default:
      return MaterialPageRoute(
          builder: (context) => UndefinedView(
                name: settings.name!,
              ));
  }
}
