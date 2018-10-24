/*
Copyright © 2017-2018,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC
All rights reserved. See LICENSE file and DISCLAIMER for more details.
*/
#pragma once

#include "HelicsPrimaryTypes.hpp"
#include "ValueFederate.hpp"
#include "../core/core-exceptions.hpp"

namespace helics
{
class Publication
{
  private:
    ValueFederate *fed = nullptr;  //!< the federate construct to interact with
    interface_handle handle;  //!< the internal id of the publication
    int referenceIndex = -1;  //!< an index used for callback lookup
    void *dataReference = nullptr;  //!< pointer to a piece of containing data
    double delta = -1.0;  //!< the minimum change to publish
    helics_type_t pubType;  //!< the type of publication
    bool changeDetectionEnabled = false;  //!< the change detection is enabled
    mutable defV prevValue;  //!< the previous value of the publication
    std::string key_;  //!< the name of the publication
    std::string units_;  //!< the defined units of the publication
  public:
    Publication () = default;
    /** base constructor for a publication
    @param a valueFed a pointer the link valueFederate
    @param id the interface_handle from the core
    @param key the identifier for the publication
    @param type the type of the publication
    @param units, an optional string defining the units*/
    Publication(ValueFederate *valueFed,interface_handle id,
        const std::string &key,
        const std::string &type,
        const std::string &units);

    /** base constructor for a publication
    @tparam a valueFed a pointer of some kind to a value federate (any dereferencable type with * and -> operator
    that results in a valueFederate object
    @param key the identifier for the publication
    @param type the type of the publication
    @param units, an optional string defining the units*/
    template <class FedPtr>
    Publication (FedPtr valueFed,
                     const std::string &key,
                     const std::string &type,
                     const std::string &units = std::string ())
        :Publication(valueFed->registerPublication(key,type,units))
    {
        static_assert (std::is_base_of<ValueFederate, std::remove_reference_t<decltype (*valueFed)>>::value,
                       "first argument must be a pointer to a ValueFederate");
    }
    /** base constructor for a publication
    @param locality either GLOBAL or LOCAL, LOCAL prepends the federate name to create a global identifier
    @param valueFed a pointer to a value federate
    @param key the identifier for the publication
    @param type the type of the publication
    @param units, an optional string defining the units*/
    Publication(interface_visibility locality,
        ValueFederate *valueFed,
        const std::string &key,
        const std::string &type,
        const std::string &units = std::string());
    /** base constructor for a publication
    @param locality either GLOBAL or LOCAL, LOCAL prepends the federate name to create a global identifier
    @tparam valueFed a pointer of some kind to a value federate (any dereferencable type with * and -> operator
    that results in a valueFederate object
    @param key the identifier for the publication
    @param type the type of the publication
    @param units, an optional string defining the units*/
    template <class FedPtr>
    Publication (interface_visibility locality,
            FedPtr & valueFed,
                     const std::string &key,
                     const std::string &type,
                     const std::string &units = std::string ())
        : Publication(locality, std::addressof (*valueFed), key, type, units)
    {
        static_assert (std::is_base_of<ValueFederate, std::remove_reference_t<decltype (*valueFed)>>::value,
            "first argument must be a pointer to a ValueFederate");
    }
 
	 /**constructor to build a publication object
   @param[in] valueFed  the ValueFederate to use
   @param[in] key the identifier for the publication
   @param type_ the defined type of the publication
   @param[in] units the units associated with a Federate
   */
    Publication (ValueFederate *valueFed,
                 const std::string &key,
                 helics_type_t type,
                 const std::string &units = std::string ())
        : Publication(valueFed, key, typeNameStringRef (type), units)
    {
    }
    /**constructor to build a publication object
    @param[in] valueFed  the ValueFederate to use
    @param[in] key the identifier for the publication
    @param type_ the defined type of the publication
    @param[in] units the units associated with a Federate
    */
    template <class FedPtr>
    Publication (FedPtr &valueFed,
                 const std::string &key,
                 helics_type_t type,
                 const std::string &units = std::string ())
        : Publication (valueFed, key, typeNameStringRef (type), units)
    {
    }
    /**constructor to build a publication object
    @param locality  set to global for a global publication or local for a local one
    @param[in] valueFed  the ValueFederate to use
    @param[in] key the identifier for the publication
    @param type_ the defined type of the publication
    @param[in] units the units associated with a Federate
    */
    Publication (interface_visibility locality,
                 ValueFederate *valueFed,
                 const std::string &key,
                 helics_type_t type,
                 const std::string &units = std::string ())
        : Publication (locality, valueFed, key, typeNameStringRef (type), units)
    {
    }

    /**constructor to build a publication object
    @param locality  set to global for a global publication or local for a local one
    @param[in] valueFed  the ValueFederate to use
    @param[in] key the identifier for the publication
    @param type_ the defined type of the publication
    @param[in] units the units associated with a Federate
    */
    template <class FedPtr>
    Publication (interface_visibility locality,
                 FedPtr &valueFed,
                 const std::string &key,
                 helics_type_t type,
                 const std::string &units = std::string ())
        : Publication(locality, valueFed, key, typeNameStringRef (type), units)
    {
    }

    /** get the publication id that can be used to make the function calls from a Value Federate
     */
    interface_handle getHandle () const { return handle; }

	 /** check if the Publication links to a valid operation*/
    bool isValid () const { return handle.isValid (); }
    bool operator== (const Publication &pub) const { return (handle == pub.handle); }
    bool operator!= (const Publication &pub) const { return (handle != pub.handle); }
    bool operator< (const Publication &pub) const { return (handle < pub.handle); }

    /** get the key for the publication*/
    const std::string &getKey () const { return fed->getPublicationKey (*this); }
    /** get the key for the publication*/
    const std::string &getName () const { return key_; }
    /** get the type for the publication*/
    const std::string &getType () const { return fed->getPublicationType (*this); }
    /** get the units of the publication*/
    const std::string &getUnits () const { return units_; }
    /** add a target to the publication*/
    void addTarget (const std::string &target) { fed->addTarget (*this, target); }

    /** send a value for publication
    @param[in] val the value to publish*/
    void publish (double val) const;
    void publish (int64_t val) const;
    void publish (const char *val) const;
    void publish (const std::string &val) const;
    void publish (const std::vector<double> &val) const;
    void publish (const std::vector<std::complex<double>> &val) const;
    void publish (const double *vals, int size) const;
    void publish (std::complex<double> val) const;
    void publish (const defV &val) const;
    void publish (bool val) const;
    void publish (const named_point &np) const;
    void publish (const std::string &name, double val) const;
    void publish (const char *str, double val) const;
    /** secondary publish function to allow unit conversion before publication
    @param[in] val the value to publish
    @param[in] units  the units association with the publication
    */
    template <class X>
    void publish (const X &val, const std::string & /*units*/) const
    {
        // TODO:: figure out units
        publish (val);
    }

    /** set the level by which a value must have changed to actually publish the value
     */
    void setMinimumChange (double deltaV)
    {
        if (delta < 0.0)
        {
            changeDetectionEnabled = true;
        }
        delta = deltaV;
        if (delta < 0.0)
        {
            changeDetectionEnabled = false;
        }
    }
    /** if set to false, the change detection mechanisms are not enabled
    if set to true the values will be published if there is sufficient change as specified in
    the call to setMinimumChange
    */
    void enableChangeDetection (bool enabled = true) { changeDetectionEnabled = enabled; }

  private:
    friend class ValueFederateManager;
};

/** create a pointer to a publication
@tparam X is the type of the publication
@tparam FedPtr a pointer a value Federate
@param valueFed pointer to a valid federate
@param key the identifier for the publication
@param units optional units for the publication
*/
template <class X>
typename std::enable_if_t<helicsType<X>() != helics_type_t::helicsInvalid, std::unique_ptr<Publication>>
make_publication(ValueFederate *valueFed, const std::string &key, const std::string &units = std::string())
{
    return std::make_unique<Publication>(valueFed, helicsType<X>(), key, units);
}

/** create a pointer to a publication
@tparam X is the type of the publication
@tparam FedPtr a pointer a value Federate
@param valueFed pointer to a valid federate
@param key the identifier for the publication
@param units optional units for the publication
*/
template <class X, class FedPtr>
typename std::enable_if_t<helicsType<X> () != helics_type_t::helicsInvalid, std::unique_ptr<Publication>>
make_publication (FedPtr &valueFed, const std::string &key, const std::string &units = std::string ())
{
    return std::make_unique<Publication> (valueFed, helicsType<X> (), key, units);
}

/** create a pointer to a publication
@tparam X is the type of the publication
@tparam FedPtr a pointer a value Federate
@param locality either LOCAL or GLOBAL defining whether the federate name is prepended or not
@param valueFed pointer to a valid federate
@param key the identifier for the publication
@param units optional units for the publication
*/
template <class X>
typename std::enable_if_t<helicsType<X>() != helics_type_t::helicsInvalid, std::unique_ptr<Publication>>
make_publication(interface_visibility locality,
    ValueFederate *valueFed,
    const std::string &key,
    const std::string &units = std::string())
{
    return std::make_unique<Publication>(locality, valueFed, key, helicsType<X>(), units);
}

/** create a pointer to a publication
@tparam X is the type of the publication
@tparam FedPtr a pointer a value Federate
@param locality either LOCAL or GLOBAL defining whether the federate name is prepended or not
@param valueFed pointer to a valid federate
@param key the identifier for the publication
@param units optional units for the publication
*/
template <class X, class FedPtr>
typename std::enable_if_t<helicsType<X> () != helics_type_t::helicsInvalid, std::unique_ptr<Publication>>
make_publication (interface_visibility locality,
                  FedPtr &valueFed,
                  const std::string &key,
                  const std::string &units = std::string ())
{
    return std::make_unique<Publication> (locality, valueFed, key, helicsType<X> (), units);
}

/** class to handle a publication of an arbitrary type*/
template <class X>
class PublicationT : public Publication
{
  public:
    PublicationT () = default;
    /**constructor to build a publication object
    @param[in] valueFed  the ValueFederate to use
    @param[in] key the identifier for the publication
    @param[in] units the units associated with a Federate
    */
    PublicationT(ValueFederate *valueFed, const std::string &key, const std::string &units = std::string())
        : Publication(valueFed, key, typeNameString<X>(), units)
    {
    }
    /**constructor to build a publication object
    @param[in] valueFed  the ValueFederate to use
    @param[in] key the identifier for the publication
    @param[in] units the units associated with a Federate
    */
    template <class FedPtr>
    PublicationT (FedPtr &valueFed, const std::string &key, const std::string &units = std::string ())
        : Publication (valueFed, key, typeNameString<X> (), units)
    {
    }
    /**constructor to build a publication object
    @param[in] valueFed  the ValueFederate to use
    @param[in] key the identifier for the publication
    @param[in] units the units associated with a Federate
    */
    PublicationT(interface_visibility locality,
        ValueFederate *valueFed,
        const std::string &key,
        const std::string &units = std::string())
        : Publication(locality, valueFed, key, typeNameString<X>(), units)
    {
    }
    /**constructor to build a publication object
    @param[in] valueFed  the ValueFederate to use
    @param[in] key the identifier for the publication
    @param[in] units the units associated with a Federate
    */
    template <class FedPtr>
    PublicationT (interface_visibility locality,
                  FedPtr &valueFed,
                  const std::string &key,
                  const std::string &units = std::string ())
        : Publication (locality, valueFed, key, typeNameString<X> (), units)
    {
    }
    /** send a value for publication
    @param[in] val the value to publish*/
    void publish (const X &val) const { fed->publish (id, val); }
    /** secondary publish function to allow unit conversion before publication
    @param[in] val the value to publish
    @param[in] units  the units association with the publication
    */
    void publish (const X &val, const std::string & /*units*/) const
    {
        // TODO:: figure out units
        publish (val);
    }
};

/** class to handle a publication on change for an arbitrary type
but the value is only published in the change is greater than a certain level*/
template <class X>
class PublicationOnChange : public PublicationT<X>
{
  private:
    X publishDelta;  //!< the delta on which to publish a value
    mutable X prev;  //!< the previous value
  public:
    PublicationOnChange () = default;
    /**constructor to build a publishOnChange object
    @param[in] valueFed  the ValueFederate to use
    @param[in] key the identifier for the publication
    @param[in] minChange  the minimum change required to actually publish the value
    @param[in] units the units associated with a Federate
    */
    PublicationOnChange(ValueFederate *valueFed,
        const std::string &key,
        const X &minChange,
        const std::string &units = std::string())
        : PublicationT<X>(valueFed, key, units), publishDelta(minChange), prev(X())
    {
    }
    /**constructor to build a publishOnChange object
    @param[in] valueFed  the ValueFederate to use
    @param[in] key the identifier for the publication
    @param[in] minChange  the minimum change required to actually publish the value
    @param[in] units the units associated with a Federate
    */
    template <class FedPtr>
    PublicationOnChange (FedPtr &valueFed,
                         const std::string &key,
                         const X &minChange,
                         const std::string &units = std::string ())
        : PublicationT<X> (valueFed, key, units), publishDelta (minChange), prev (X ())
    {
    }
    /** send a value for publication
    @details the value is only published if it exceeds the specified level
    @param[in] val the value to publish*/
    virtual void publish (const X &val) const override
    {
        if (std::abs (val - prev) >= publishDelta)
        {
            prev = val;
            PublicationT<X>::publish (val);
        }
    }
};
}  // namespace helics
