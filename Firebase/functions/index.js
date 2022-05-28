const functions = require("firebase-functions");
const admin = require("firebase-admin");
const { DocumentBuilder } = require("firebase-functions/v1/firestore");
admin.initializeApp();


exports.helloWorld = functions.https.onRequest((request, response) => {
 response.send("Hello from Firebase!");
});

exports.onNodeWrite = functions.firestore.document('/Nodes/{node_id}').onWrite(async (change, context) => {
    const node_id = context.params.node_id;
    if (change.after.exists) {
        // const documentSnapshot =  change.after;
        const doc = admin.firestore().collection('Changes').doc(node_id);
        const data = change.after.data();
        return doc.set({'config' : JSON.stringify(data)});
    } else {
        return functions.logger.log("Node " + node_id + "has deleted");
    }
});


/*jsonToDocumenton and documentToJson are based on https://stackoverflow.com/questions/62246410/how-to-convert-a-firestore-document-to-plain-json-and-vice-versa*/
function jsonToDocument(value) {
    if (value == null) {
        return { "nullValue": null };
    } else if (!isNaN(value)) {
        if (value.toString().indexOf(".") != -1) {
            return { "doubleValue": value };
        } else {
            return { "integerValue": value };
        }
    } else if (value === "true" || value === "false" || typeof value == "boolean") {
        return { "booleanValue": value };
    } else if (Date.parse(value)) {
        return { "timestampValue": value };
    } else if (typeof value == "string") {
        return { "stringValue": value };
    } else if (value && value.constructor === Array) {
        return { "arrayValue": { values: value.map((v) => jsonToDocument(v)) } };
    } else if (typeof value === "object") {
        const obj = {};
        for (const o in value) {
            obj[o] = jsonToDocument(value[o]);
        }
        return { "mapValue": { fields: obj } };
    }
}

/* */
function documentToJson(fields) {
    const result = {};
    for (const f in fields) {
        const key = f;
        const value = fields[f];
        const isDocumentType = ["stringValue", "booleanValue", "doubleValue",
            "integerValue", "timestampValue", "mapValue", "arrayValue", "nullValue",
        ].find((t) => t === key);
        if (isDocumentType) {
            const item = ["stringValue", "booleanValue", "doubleValue",
                "integerValue", "timestampValue", "nullValue"]
                .find((t) => t === key);
            if (item) {
                return value;
            } else if ("mapValue" == key) {
                return documentToJson(value.fields || {});
            } else if ("arrayValue" == key) {
                const list = value.values;
                return list ? list.map((l) => documentToJson(l)) : [];
            }
        } else {
            result[key] = documentToJson(value);
        }
    }
    return result;
}
