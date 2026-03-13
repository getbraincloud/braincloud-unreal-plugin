// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudGlobalApp
{
public:
     BrainCloudGlobalApp(BrainCloudClient *client);

     /**
      * Read game's global properties
      *
      * Service Name - globalApp
      * Service Operation - READ_PROPERTIES
      *
      * @param callback The method to be invoked when the server response is received
      */
     void readProperties(IServerCallback *callback);

     /**
      * Returns a list of properties, identified by the property names provided.
      * If a property from the list isn't found, it just isn't returned (no error).
      *
      * Service Name - globalApp
      * Service Operation - READ_SELECTED_PROPERTIES
      *
      * @param propertyNames Specifies which properties to return
      * @param callback The method to be invoked when the server response is received
      */
     void readSelectedProperties(const TArray<FString> &in_propertyId, IServerCallback *callback);

     /**
      * Returns a list of properties, identified by the categories provided.
      * If a category from the list isn't found, it just isn't returned (no error).
      *
      * Service Name - globalApp
      * Service Operation - READ_PROPERTIES_CATEGORIES
      *
      * @param categories Specifies which category to return
      * @param callback The method to be invoked when the server response is received
      */
     void readPropertiesInCategories(const TArray<FString> &in_categories, IServerCallback *callback);

private:
     BrainCloudClient *_client = nullptr;
};