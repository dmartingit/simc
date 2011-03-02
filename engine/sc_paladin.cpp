// ==========================================================================
// Dedmonwakeen's DPS-DPM Simulator.
// Send questions to natehieter@gmail.com
// ==========================================================================

#include "simulationcraft.h"

// ==========================================================================
// Paladin
// ==========================================================================

namespace
{

enum seal_type_t
{
  SEAL_NONE=0,
  SEAL_OF_JUSTICE,
  SEAL_OF_INSIGHT,
  SEAL_OF_RIGHTEOUSNESS,
  SEAL_OF_TRUTH,
  SEAL_MAX
};

}

struct paladin_t : public player_t
{
  // Active
  int       active_seal;
  action_t* active_hand_of_light_proc;
  action_t* active_seal_of_insight_proc;
  action_t* active_seal_of_justice_proc;
  action_t* active_seal_of_righteousness_proc;
  action_t* active_seal_of_truth_dot;
  action_t* active_seal_of_truth_proc;
  action_t* active_seals_of_command_proc;
  action_t* ancient_fury_explosion;

  // Buffs
  buff_t* buffs_ancient_power;
  buff_t* buffs_avenging_wrath;
  buff_t* buffs_censure;
  buff_t* buffs_divine_favor;
  buff_t* buffs_divine_plea;
  buff_t* buffs_divine_purpose;
  buff_t* buffs_holy_shield;
  buff_t* buffs_inquisition;
  buff_t* buffs_judgements_of_the_bold;
  buff_t* buffs_judgements_of_the_pure;
  buff_t* buffs_judgements_of_the_wise;
  buff_t* buffs_reckoning;
  buff_t* buffs_sacred_duty;
  buff_t* buffs_the_art_of_war;
  buff_t* buffs_zealotry;

  // Gains
  gain_t* gains_divine_plea;
  gain_t* gains_judgements_of_the_bold;
  gain_t* gains_judgements_of_the_wise;
  gain_t* gains_sanctuary;
  gain_t* gains_seal_of_command_glyph;
  gain_t* gains_seal_of_insight;

  // Holy Power
  gain_t* gains_hp_crusader_strike;
  gain_t* gains_hp_holy_shock;
  gain_t* gains_hp_blessed_life;
  gain_t* gains_hp_tower_of_radiance;
  gain_t* gains_hp_hammer_of_the_righteous;
  gain_t* gains_hp_divine_plea;
  gain_t* gains_hp_pursuit_of_justice;
  gain_t* gains_hp_zealotry;
  gain_t* gains_hp_divine_storm;

  // Passives
  struct passives_t
  {
    mastery_t* divine_bulwark;
    mastery_t* hand_of_light;
    passive_spell_t* judgements_of_the_bold; // passive stuff is hidden here because spells
    passive_spell_t* judgements_of_the_wise; // can only have three effects
    passive_spell_t* plate_specialization;
    passive_spell_t* sheath_of_light;
    passive_spell_t* touched_by_the_light;
    passive_spell_t* two_handed_weapon_spec;
    passive_spell_t* vengeance;
    passives_t() { memset( ( void* ) this, 0x0, sizeof( passives_t ) ); }
  };
  passives_t passives;

  // Pets
  pet_t* guardian_of_ancient_kings;

  // Procs
  proc_t* procs_parry_haste;

  // Spells
  struct spells_t
  {
    active_spell_t* divine_favor;
    active_spell_t* guardian_of_ancient_kings_ret;
    spells_t() { memset( ( void* ) this, 0x0, sizeof( spells_t ) ); }
  } spells;

  // Talents
  struct talents_t
  {
    // holy
    int aura_mastery;
    int beacon_of_light;
    int blessed_life;
    int clarity_of_purpose;
    int conviction;
    int daybreak;
    int denounce;
    int enlightened_judgements;
    int infusion_of_light;
    int last_word;
    int light_of_dawn;
    int paragon_of_virtue;
    int protector_of_the_innocent;
    int sacred_cleansing;
    int speed_of_light;
    int tower_of_radiance;
    talent_t* arbiter_of_the_light;
    talent_t* blazing_light;
    talent_t* divine_favor;
    talent_t* judgements_of_the_pure;

    // prot
    int ardent_defender;
    int divine_guardian;
    int divinity;
    int eternal_glory;
    int guarded_by_the_light;
    int vindication;
    talent_t* grand_crusader;
    talent_t* hallowed_ground;
    talent_t* hammer_of_the_righteous;
    talent_t* holy_shield;
    talent_t* improved_hammer_of_justice;
    talent_t* judgements_of_the_just;
    talent_t* reckoning;
    talent_t* sacred_duty;
    talent_t* sanctuary;
    talent_t* seals_of_the_pure;
    talent_t* shield_of_the_righteous;
    talent_t* shield_of_the_templar;
    talent_t* toughness;
    talent_t* wrath_of_the_lightbringer;

    // ret
    int acts_of_sacrifice;
    int eye_for_an_eye;
    int guardians_favor;
    int improved_judgement;
    int long_arm_of_the_law;
    int pursuit_of_justice;
    int repentance;
    int selfless_healer;
    talent_t* communion;
    talent_t* crusade;
    talent_t* divine_purpose;
    talent_t* divine_storm;
    talent_t* inquiry_of_faith;
    talent_t* rule_of_law;
    talent_t* sanctified_wrath;
    talent_t* sanctity_of_battle;
    talent_t* seals_of_command;
    talent_t* the_art_of_war;
    talent_t* zealotry;

    talents_t() { memset( ( void* ) this, 0x0, sizeof( talents_t ) ); }
  };
  talents_t talents;

  // Glyphs
  struct glyphs_t
  {
    // prime
    glyph_t* crusader_strike;
    glyph_t* divine_favor;
    glyph_t* exorcism;
    glyph_t* hammer_of_the_righteous;
    glyph_t* holy_shock;
    glyph_t* judgement;
    glyph_t* seal_of_truth;
    glyph_t* shield_of_the_righteous;
    glyph_t* templars_verdict;

    // major
    glyph_t* ascetic_crusader;
    glyph_t* consecration;
    glyph_t* divine_plea;
    glyph_t* focused_shield;
    glyph_t* hammer_of_wrath;

    glyphs_t() { memset( ( void* ) this, 0x0, sizeof( glyphs_t ) ); }
  };
  glyphs_t glyphs;

  int ret_pvp_gloves;

  paladin_t( sim_t* sim, const std::string& name, race_type r = RACE_NONE ) : player_t( sim, PALADIN, name, r )
  {
    if ( race == RACE_NONE ) race = RACE_TAUREN;

    tree_type[ PALADIN_HOLY        ] = TREE_HOLY;
    tree_type[ PALADIN_PROTECTION  ] = TREE_PROTECTION;
    tree_type[ PALADIN_RETRIBUTION ] = TREE_RETRIBUTION;

    active_seal = SEAL_NONE;

    active_seals_of_command_proc      = 0;
    active_seal_of_justice_proc       = 0;
    active_seal_of_insight_proc       = 0;
    active_seal_of_righteousness_proc = 0;
    active_seal_of_truth_proc         = 0;
    active_seal_of_truth_dot          = 0;

    ret_pvp_gloves = 0;

    create_talents();
    create_glyphs();
    create_options();
  }

  virtual void      init_base();
  virtual void      init_gains();
  virtual void      init_procs();
  virtual void      init_scaling();
  virtual void      init_buffs();
  virtual void      init_talents();
  virtual void      init_spells();
  virtual void      init_actions();
  virtual void      reset();
  virtual double    composite_attribute_multiplier( int attr ) SC_CONST;
  virtual double    composite_attack_expertise() SC_CONST;
  virtual double    composite_spell_power( const school_type school ) SC_CONST;
  virtual double    composite_tank_block() SC_CONST;
  virtual double    composite_tank_crit( const school_type school ) SC_CONST;
  virtual double    matching_gear_multiplier( const attribute_type attr ) SC_CONST;
  virtual action_t* create_action( const std::string& name, const std::string& options_str );
  virtual int       decode_set( item_t& item );
  virtual int       primary_resource() SC_CONST { return RESOURCE_MANA; }
  virtual int       primary_role() SC_CONST;
  virtual void      regen( double periodicity );
  virtual double      assess_damage( double amount, const school_type school, int    dmg_type, int result, action_t* a );
  virtual cooldown_t* get_cooldown( const std::string& name );
  virtual pet_t*    create_pet    ( const std::string& name, const std::string& type = std::string() );
  virtual void      create_pets   ();
  virtual void      combat_begin();

  int               holy_power_stacks() SC_CONST;
  double            get_divine_bulwark() SC_CONST;
  double            get_hand_of_light() SC_CONST;
};

namespace { // ANONYMOUS NAMESPACE ==========================================

// trigger_hand_of_light ====================================================

static void trigger_hand_of_light( action_t* a )
{
  paladin_t* p = a -> player -> cast_paladin();

  if ( p -> primary_tree() == TREE_RETRIBUTION )
  {
    p -> active_hand_of_light_proc -> base_dd_max = p -> active_hand_of_light_proc-> base_dd_min = a -> direct_dmg;
    p -> active_hand_of_light_proc -> execute();
  }
}

// Guardian of Ancient Kings Pet ============================================

// TODO: melee attack
struct guardian_of_ancient_kings_ret_t : public pet_t
{
  attack_t* melee;

  struct melee_t : public attack_t
  {
    paladin_t* owner;

    melee_t( player_t *p )
      : attack_t( "melee", p, RESOURCE_NONE, SCHOOL_PHYSICAL ), owner( 0 )
    {
      weapon = &( p -> main_hand_weapon );
      base_execute_time = weapon -> swing_time;
      weapon_multiplier = 1.0;
      background = true;
      repeating  = true;
      trigger_gcd = 0;
      base_cost   = 0;

      owner = p -> cast_pet() -> owner -> cast_paladin();
    }

    virtual void execute()
    {
      attack_t::execute();
      if ( result_is_hit() )
      {
        owner -> buffs_ancient_power -> trigger();
      }
    }
  };

  guardian_of_ancient_kings_ret_t( sim_t *sim, paladin_t *p )
    : pet_t( sim, p, "guardian_of_ancient_kings", true ), melee( 0 )
  {
    main_hand_weapon.type = WEAPON_BEAST;
    main_hand_weapon.swing_time = 2.0;
    main_hand_weapon.min_dmg = 5500; // TODO
    main_hand_weapon.max_dmg = 7000; // TODO
  }

  virtual void init_base()
  {
    pet_t::init_base();
    melee = new melee_t( this );
  }

  virtual void dismiss()
  {
    pet_t::dismiss();
    owner -> cast_paladin() -> ancient_fury_explosion -> execute();
  }

  virtual void schedule_ready( double delta_time=0, bool waiting=false )
  {
    pet_t::schedule_ready( delta_time, waiting );
    if ( ! melee -> execute_event ) melee -> execute();
  }
};

// =========================================================================
// Paladin Attacks
// =========================================================================

struct paladin_attack_t : public attack_t
{
  bool trigger_seal;
  bool spell_haste; // Some attacks (CS w/ sanctity of battle, censure) use spell haste. sigh.
  double jotp_haste;
  bool trigger_dp;

  paladin_attack_t( const char* n, paladin_t* p, const school_type s=SCHOOL_PHYSICAL, int t=TREE_NONE, bool special=true, bool use2hspec=true )
    : attack_t( n, p, RESOURCE_MANA, s, t, special ),
      trigger_seal( false ), spell_haste( false ),  jotp_haste( 1.0 ), trigger_dp( false )
  {
    initialize_( use2hspec );
  }

  paladin_attack_t( const char* n, uint32_t id, paladin_t* p, bool use2hspec=true, bool special=true )
    : attack_t( n, id, p, TREE_NONE, special ),
      trigger_seal( false ), spell_haste( false ), jotp_haste( 1.0 ), trigger_dp( false )
  {
    initialize_( use2hspec );
  }

  paladin_attack_t( const char* n, const char* sname, paladin_t* p, bool use2hspec=true, bool special=true )
    : attack_t( n, sname, p, TREE_NONE, special ),
      trigger_seal( false ), spell_haste( false ), jotp_haste( 1.0 ), trigger_dp( false )
  {
    initialize_( use2hspec );
  }

  void initialize_( bool use2hspec )
  {
    may_crit = true;
    if ( use2hspec && p() -> primary_tree() == TREE_RETRIBUTION && p() -> main_hand_weapon.group() == WEAPON_2H )
    {
      base_multiplier *= 1.0 + p() -> passives.two_handed_weapon_spec->base_value( E_APPLY_AURA, A_MOD_DAMAGE_PERCENT_DONE );
    }

    base_multiplier *= 1.0 + 0.01 * p() -> talents.communion -> effect_base_value( 3 );

    if ( p() -> set_bonus.tier10_2pc_melee() )
      base_multiplier *= 1.05;

    if ( p() -> talents.judgements_of_the_pure -> rank() )
      jotp_haste = 1.0 / ( 1.0 + p() -> buffs_judgements_of_the_pure -> base_value( E_APPLY_AURA, A_HASTE_ALL ) );
  }

  paladin_t* p() SC_CONST
  {
    return static_cast<paladin_t*>( player );
  }

  virtual double haste() SC_CONST
  {
    paladin_t* p = player -> cast_paladin();
    double h = spell_haste ? p -> composite_spell_haste() : attack_t::haste();
    if ( p -> buffs_judgements_of_the_pure -> up() )
    {
      h *= jotp_haste;
    }
    return h;
  }

  virtual void execute()
  {
    attack_t::execute();
    if ( result_is_hit() )
    {

      if ( trigger_seal )
      {
        paladin_t* p = player -> cast_paladin();
        switch ( p -> active_seal )
        {
        case SEAL_OF_JUSTICE:
          p -> active_seal_of_justice_proc       -> execute();
          break;
        case SEAL_OF_INSIGHT:
          p -> active_seal_of_insight_proc       -> execute();
          break;
        case SEAL_OF_RIGHTEOUSNESS:
          p -> active_seal_of_righteousness_proc -> execute();
          break;
        case SEAL_OF_TRUTH:
          if ( p -> buffs_censure -> stack() >= 1 ) p -> active_seal_of_truth_proc -> execute();
          break;
        default:
          ;
        }

        if ( p -> active_seal != SEAL_OF_INSIGHT && p -> talents.seals_of_command->rank() )
        {
          p -> active_seals_of_command_proc -> execute();
        }

        // TODO: does the censure stacking up happen before or after the SoT proc?
        if ( p -> active_seal == SEAL_OF_TRUTH )
        {
          p -> active_seal_of_truth_dot -> execute();
        }

        // It seems like it's the seal-triggering attacks that stack up ancient power		
        if ( ! p -> guardian_of_ancient_kings -> sleeping )		
        {		
          p -> buffs_ancient_power -> trigger();		
        }
      }
      if ( trigger_dp )
        p() -> buffs_divine_purpose -> trigger();
    }
  }

  virtual void player_buff()
  {
    paladin_t* p = player -> cast_paladin();
    attack_t::player_buff();
    player_multiplier *= 1.0 + p -> buffs_avenging_wrath -> value();
    if ( school == SCHOOL_HOLY )
    {
      player_multiplier *= 1.0 + p -> buffs_inquisition -> value();
    }
  }


  virtual double cost() SC_CONST
  {
    paladin_t* p = player -> cast_paladin();

    if ( resource == RESOURCE_HOLY_POWER )
    {
      if ( p -> buffs_divine_purpose -> check() )
        return 0;

      return std::max( base_cost, p -> resource_current[ RESOURCE_HOLY_POWER ] );
    }

    return attack_t::cost();
   }

  virtual void consume_resource()
  {
    attack_t::consume_resource();

    paladin_t* p = player -> cast_paladin();

    p -> buffs_divine_purpose -> expire();
  }
};

// Melee Attack ============================================================

struct melee_t : public paladin_attack_t
{
  melee_t( paladin_t* p )
    : paladin_attack_t( "melee", p, SCHOOL_PHYSICAL, true/*2hspec*/, false/*special*/ )
  {
    special           = false;
    trigger_seal      = true;
    background        = true;
    repeating         = true;
    trigger_gcd       = 0;
    base_cost         = 0;
    weapon            = &( p -> main_hand_weapon );
    base_execute_time = p -> main_hand_weapon.swing_time;
  }

  virtual double execute_time() SC_CONST
  {
    if ( ! player -> in_combat ) return 0.01;
    return paladin_attack_t::execute_time();
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    paladin_attack_t::execute();
    if ( result_is_hit() )
    {
      p -> buffs_the_art_of_war -> trigger();
    }
    if ( !proc && p -> buffs_reckoning -> up() )
    {
      p -> buffs_reckoning -> decrement();
      proc = true;
      paladin_attack_t::execute();
      proc = false;
    }
  }
};

// Auto Attack =============================================================

struct auto_attack_t : public paladin_attack_t
{
  auto_attack_t( paladin_t* p, const std::string& options_str )
    : paladin_attack_t( "auto_attack", p )
  {
    assert( p -> main_hand_weapon.type != WEAPON_NONE );
    p -> main_hand_attack = new melee_t( p );

    trigger_gcd = 0;
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    p -> main_hand_attack -> schedule_execute();
  }

  virtual bool ready()
  {
    paladin_t* p = player -> cast_paladin();
    if ( p -> buffs.moving -> check() ) return false;
    return( p -> main_hand_attack -> execute_event == 0 ); // not swinging
  }
};

// Ancient Fury =============================================================

struct ancient_fury_t : public paladin_attack_t
{
  ancient_fury_t( paladin_t* p )
    : paladin_attack_t( "ancient_fury", 86704, p, false )
  {
    // TODO meteor stuff
    background = true;
  }

  virtual void execute()
  {
    paladin_attack_t::execute();
    player -> cast_paladin() -> buffs_ancient_power -> expire();
  }

  virtual void player_buff()
  {
    paladin_attack_t::player_buff();
    player_multiplier *= player -> cast_paladin() -> buffs_ancient_power -> stack();
  }
};

// Avengers Shield =========================================================

struct avengers_shield_t : public paladin_attack_t
{
  avengers_shield_t( paladin_t* p, const std::string& options_str )
    : paladin_attack_t( "avengers_shield", "Avenger's Shield", p )
  {
    parse_options( NULL, options_str );

    trigger_seal = false;
    aoe          = 2;
    may_parry    = false;
    may_dodge    = false;
    may_block    = false;

    base_spell_power_multiplier  = direct_power_mod;
    base_attack_power_multiplier = extra_coeff();
    direct_power_mod = 1.0;

    if ( p -> glyphs.focused_shield -> ok() )
    {
      aoe             += (int) p -> glyphs.focused_shield -> mod_additive( P_TARGET );
      base_multiplier *= 1.0 + p -> glyphs.focused_shield -> mod_additive( P_GENERIC );
    }
  }
};

// Crusader Strike ==========================================================

struct crusader_strike_t : public paladin_attack_t
{
  double base_cooldown;
  crusader_strike_t( paladin_t* p, const std::string& options_str )
    : paladin_attack_t( "crusader_strike", "Crusader Strike", p ), base_cooldown( 0 )
  {
    parse_options( NULL, options_str );

    spell_haste  = true;
    trigger_seal = true;    

    // JotW decreases the CD by 1.5 seconds for Prot Pallies, but it's not in the tooltip
    cooldown -> duration += p -> passives.judgements_of_the_wise -> mod_additive( P_COOLDOWN );
    base_cooldown         = cooldown -> duration;

    base_crit       += p -> talents.rule_of_law -> mod_additive( P_CRIT );
    base_crit       += p -> glyphs.crusader_strike -> mod_additive( P_CRIT );
    base_multiplier *= 1.0 + p -> talents.crusade -> mod_additive( P_GENERIC )
                       + p -> talents.wrath_of_the_lightbringer-> mod_additive( P_GENERIC ) // TODO how do they stack?
                       + 0.10 * p -> set_bonus.tier11_2pc_tank()
                       + 0.05 * p -> ret_pvp_gloves;
    base_cost       *= 1.0 + p -> glyphs.ascetic_crusader -> mod_additive( P_RESOURCE_COST );
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    paladin_attack_t::execute();
    
    if ( result_is_hit() )
    {
      p -> resource_gain( RESOURCE_HOLY_POWER, p -> buffs_zealotry -> up() ? 3 : 1,
                                p -> gains_hp_crusader_strike );

      trigger_hand_of_light( this );
      if ( sim -> roll( p -> talents.grand_crusader -> proc_chance() ) )
      {
        p -> get_cooldown( "avengers_shield" ) -> reset();
      }
    }
  }

  virtual void update_ready()
  {
    paladin_t* p = player -> cast_paladin();
    if ( p -> talents.sanctity_of_battle -> rank() )
    {
      cooldown -> duration = base_cooldown * haste();
      if ( sim -> log ) log_t::output( sim, "%s %s cooldown is %.2f", p -> name(), name(),  cooldown -> duration );
    }

    paladin_attack_t::update_ready();
  }
};

// Divine Storm ============================================================

struct divine_storm_t : public paladin_attack_t
{
  double base_cooldown;
  divine_storm_t( paladin_t* p, const std::string& options_str )
    : paladin_attack_t( "divine_storm", "Divine Storm", p ), base_cooldown( 0 )
  {
    check_talent( p -> talents.divine_storm -> rank() );

    parse_options( NULL, options_str );

    aoe               = -1;
    spell_haste       = true;
    trigger_dp        = true;
    trigger_seal      = false;
    base_cooldown     = cooldown -> duration;
  }

  virtual void update_ready()
  {
    paladin_t* p = player -> cast_paladin();
    if ( p -> talents.sanctity_of_battle -> rank() )
    {
      cooldown -> duration = base_cooldown * haste();
    }

    paladin_attack_t::update_ready();
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    paladin_attack_t::execute();
    if ( result_is_hit() )
    {
      trigger_hand_of_light( this );
      if ( p -> ptr )
        if ( target -> cast_target() -> adds_nearby >= 4 )
          {
            p -> resource_gain( RESOURCE_HOLY_POWER, 1,
                                      p -> gains_hp_divine_storm );
          }
    }
  }
};

// Hammer of Justice =======================================================

struct hammer_of_justice_t : public paladin_attack_t
{
  hammer_of_justice_t( paladin_t* p, const std::string& options_str )
    : paladin_attack_t( "hammer_of_justice", "Hammer of Justice", p )
  {
    parse_options( NULL, options_str );

    cooldown -> duration += p -> talents.improved_hammer_of_justice -> mod_additive( P_COOLDOWN );
  }
};

// Hammer of the Righteous =================================================

struct hammer_of_the_righteous_aoe_t : public paladin_attack_t
{
  hammer_of_the_righteous_aoe_t( paladin_t* p )
    : paladin_attack_t( "hammer_of_the_righteous_aoe", 88263, p, false )
  {
    may_dodge = false;
    may_parry = false;
    may_miss  = false;
    aoe       = -1;

    direct_power_mod = extra_coeff();
    base_crit       += p -> talents.rule_of_law -> mod_additive( P_CRIT );
    base_multiplier *= 1.0 + p -> talents.crusade -> mod_additive( P_GENERIC );
    base_multiplier *= 1.0 + p -> glyphs.hammer_of_the_righteous -> mod_additive( P_GENERIC );
  }
};

struct hammer_of_the_righteous_t : public paladin_attack_t
{
  hammer_of_the_righteous_aoe_t *proc;

  hammer_of_the_righteous_t( paladin_t* p, const std::string& options_str )
    : paladin_attack_t( "hammer_of_the_righteous", "Hammer of the Righteous", p, false ), proc( 0 )
  {
    check_talent( p -> talents.hammer_of_the_righteous -> rank() );

    parse_options( NULL, options_str );

    proc = new hammer_of_the_righteous_aoe_t( p );

    base_multiplier *= 1.0 + p -> talents.crusade -> mod_additive( P_GENERIC );
    base_multiplier *= 1.0 + p -> glyphs.hammer_of_the_righteous -> mod_additive( P_GENERIC );
  }

  virtual void execute()
  {
    paladin_attack_t::execute();
    if ( result_is_hit() )
    {
      proc -> execute();
      if ( sim -> roll( p() -> talents.grand_crusader -> proc_chance() ) )
      {
        p() -> get_cooldown( "avengers_shield" ) -> reset();
      }
    }
  }
};

// Hammer of Wrath =========================================================

struct hammer_of_wrath_t : public paladin_attack_t
{
  hammer_of_wrath_t( paladin_t* p, const std::string& options_str )
    : paladin_attack_t( "hammer_of_wrath", "Hammer of Wrath", p, false )
  {
    parse_options( NULL, options_str );

    may_parry    = false;
    may_dodge    = false;
    may_block    = false;
    trigger_dp   = true;
    trigger_seal = 1; // TODO: only SoT or all seals?

    base_crit += p -> talents.sanctified_wrath -> mod_additive( P_CRIT )
                 + p -> talents.wrath_of_the_lightbringer -> mod_additive( P_CRIT );
    base_cost *= p -> glyphs.hammer_of_wrath -> mod_additive( P_RESOURCE_COST );
    
    base_spell_power_multiplier  = direct_power_mod;
    base_attack_power_multiplier = extra_coeff();
    direct_power_mod             = 1.0;
  }

  virtual bool ready()
  {
    paladin_t* p = player -> cast_paladin();
    if ( target -> health_percentage() > 20 && ! ( p -> talents.sanctified_wrath -> rank() && p -> buffs_avenging_wrath -> check() ) )
      return false;

    return paladin_attack_t::ready();
  }
};

// Hand of Light proc ======================================================

struct hand_of_light_proc_t : public attack_t
{
  hand_of_light_proc_t( paladin_t* p )
    : attack_t( "hand_of_light", p, RESOURCE_NONE, SCHOOL_HOLY, TREE_RETRIBUTION, true )
  {
    may_crit    = false;
    may_miss    = false;
    may_dodge   = false;
    may_parry   = false;
    proc        = true;
    background  = true;
    trigger_gcd = 0;
  }

  virtual void player_buff()
  {
    attack_t::player_buff();
    paladin_t* p = player -> cast_paladin();
    // not *= since we don't want to double dip, just calling base to initialize variables
    player_multiplier = p -> get_hand_of_light();
    player_multiplier *= 1.0 + p -> buffs_inquisition -> value();
  }

  virtual void target_debuff( player_t* t, int dmg_type )
  {
    attack_t::target_debuff( t, dmg_type );
    target_multiplier = 1.0;

    // TODO: The HoL proc isn't affected by CoE, unknown if this is a bug or intended.
    // not *= since we don't want to double dip, just calling base to initialize variables
    //target_multiplier = 1.0 + ( std::max( t -> debuffs.curse_of_elements  -> value(),
    //                            std::max( t -> debuffs.earth_and_moon     -> value(),
    //                                      t -> debuffs.ebon_plaguebringer -> value() ) ) * 0.01 );
  }
};

// Paladin Seals ============================================================

struct paladin_seal_t : public paladin_attack_t
{
  int seal_type;

  paladin_seal_t( paladin_t* p, const char* n, int st, const std::string& options_str )
    : paladin_attack_t( n, p ), seal_type( st )
  {
    parse_options( NULL, options_str );

    harmful    = false;
    base_cost  = p -> resource_base[ RESOURCE_MANA ] * 0.14;
  }

  virtual void execute()
  {
    if ( sim -> log ) log_t::output( sim, "%s performs %s", player -> name(), name() );
    consume_resource();
    paladin_t* p = player -> cast_paladin();
    p -> active_seal = seal_type;
  }

  virtual bool ready()
  {
    paladin_t* p = player -> cast_paladin();
    if ( p -> active_seal == seal_type ) return false;
    return paladin_attack_t::ready();
  }
};

// Seal of Insight ==========================================================

struct seal_of_insight_proc_t : public paladin_attack_t
{
  seal_of_insight_proc_t( paladin_t* p ) :
    paladin_attack_t( "seal_of_insight", p, SCHOOL_HOLY )
  {
    background  = true;
    proc        = true;
    trigger_gcd = 0;

    base_multiplier *= 1.0 + 0.10 * p -> set_bonus.tier10_4pc_melee();

    base_attack_power_multiplier = 0.15;
    base_spell_power_multiplier  = 0.15;

    weapon            = &( p -> main_hand_weapon );
    weapon_multiplier = 0.0;
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    p -> resource_gain( RESOURCE_HEALTH, total_power() );
    p -> resource_gain( RESOURCE_MANA, p -> resource_max[ RESOURCE_MANA ] * effect_base_value( 2 ) / 100.0, p -> gains_seal_of_insight );
  }
};

struct seal_of_insight_judgement_t : public paladin_attack_t
{
  seal_of_insight_judgement_t( paladin_t* p ) :
    paladin_attack_t( "judgement_of_insight", p, SCHOOL_HOLY )
  {
    background = true;

    may_parry = false;
    may_dodge = false;
    may_block = false;

    base_crit       += p -> talents.arbiter_of_the_light -> mod_additive( P_CRIT );
    base_multiplier *= 1.0 + 0.10 * p -> set_bonus.tier10_4pc_melee()
                       + 0.01 * p -> talents.wrath_of_the_lightbringer->effect_base_value( 1 );
    base_multiplier *= 1.0 + p -> glyphs.judgement -> mod_additive( P_GENERIC );

    direct_power_mod             = 1.0;
    base_spell_power_multiplier  = 0.25;
    base_attack_power_multiplier = 0.16;

    weapon            = &( p -> main_hand_weapon );
    weapon_multiplier = 0.0;

    cooldown -> duration = 8;
  }
};

// Seal of Justice ==========================================================

struct seal_of_justice_proc_t : public paladin_attack_t
{
  seal_of_justice_proc_t( paladin_t* p ) :
    paladin_attack_t( "seal_of_justice", p, SCHOOL_HOLY )
  {
    background        = true;
    proc              = true;
    trigger_gcd       = 0;
    weapon            = &( p -> main_hand_weapon );
    weapon_multiplier = 0.0;
  }

  virtual void execute()
  {
    // No need to model stun
  }
};

struct seal_of_justice_judgement_t : public paladin_attack_t
{
  seal_of_justice_judgement_t( paladin_t* p ) :
    paladin_attack_t( "judgement_of_justice", p, SCHOOL_HOLY )
  {
    background = true;
    may_parry  = false;
    may_dodge  = false;
    may_block  = false;

    base_crit       += p -> talents.arbiter_of_the_light -> mod_additive( P_CRIT );
    base_multiplier *= 1.0 + 0.01 * p -> talents.wrath_of_the_lightbringer -> effect_base_value( 1 )
                       + 0.10 * p -> set_bonus.tier10_4pc_melee();
    base_multiplier *= 1.0 + p -> glyphs.judgement -> mod_additive( P_GENERIC );

    direct_power_mod             = 1.0;
    base_spell_power_multiplier  = 0.25;
    base_attack_power_multiplier = 0.16;

    weapon            = &( p -> main_hand_weapon );
    weapon_multiplier = 0.0;

    cooldown -> duration = 8;
  }
};

// Seal of Righteousness ====================================================

struct seal_of_righteousness_proc_t : public paladin_attack_t
{
  seal_of_righteousness_proc_t( paladin_t* p ) :
    paladin_attack_t( "seal_of_righteousness", p, SCHOOL_HOLY )
  {
    background  = true;
    may_crit    = false;
    proc        = true;
    trigger_gcd = 0;

    aoe              = ( int ) p -> talents.seals_of_command -> mod_additive( P_TARGET );
    base_multiplier *= p -> main_hand_weapon.swing_time; // Note that tooltip changes with haste, but actual damage doesn't
    base_multiplier *= 1.0 + ( 0.01 * p -> talents.seals_of_the_pure -> effect_base_value( 1 ) +
                               0.10 * p -> set_bonus.tier10_4pc_melee() );

    direct_power_mod             = 1.0;
    base_attack_power_multiplier = 0.011;
    base_spell_power_multiplier  = 2 * base_attack_power_multiplier;

    weapon            = &( p -> main_hand_weapon );
    weapon_multiplier = 0.0;
  }
};

struct seal_of_righteousness_judgement_t : public paladin_attack_t
{
  seal_of_righteousness_judgement_t( paladin_t* p ) :
    paladin_attack_t( "judgement_of_righteousness", p, SCHOOL_HOLY )
  {
    background = true;
    may_parry  = false;
    may_dodge  = false;
    may_block  = false;
    may_crit   = false;

    base_crit       += p -> talents.arbiter_of_the_light -> mod_additive( P_CRIT );
    base_multiplier *= 1.0 + 0.10 * p -> set_bonus.tier10_4pc_melee()
                       + 0.01 * p -> talents.wrath_of_the_lightbringer->effect_base_value( 1 );
    base_multiplier *= 1.0 + p -> glyphs.judgement -> mod_additive( P_GENERIC );

    base_dd_min = base_dd_max    = 1.0;
    direct_power_mod             = 1.0;
    base_spell_power_multiplier  = 0.32;
    base_attack_power_multiplier = 0.20;

    weapon            = &( p -> main_hand_weapon );
    weapon_multiplier = 0.0;

    cooldown -> duration = 8;
  }
};

// Seal of Truth ============================================================

struct seal_of_truth_dot_t : public paladin_attack_t
{
  seal_of_truth_dot_t( paladin_t* p )
    : paladin_attack_t( "censure", 31803, p, false/*TODO: check 2hspec*/ )
  {
    background       = true;
    proc             = true;
    hasted_ticks     = true;
    spell_haste      = true;
    tick_may_crit    = true;
    may_crit         = false;
    dot_behavior     = DOT_REFRESH;

    base_spell_power_multiplier  = tick_power_mod;
    base_attack_power_multiplier = extra_coeff();
    tick_power_mod               = 1.0;

    // For some reason, SotP is multiplicative with 4T10 for the procs but additive for the DoT
    base_multiplier *= 1.0 + ( 0.01 * p -> talents.seals_of_the_pure -> effect_base_value( 1 ) +
                               p -> talents.inquiry_of_faith -> mod_additive( P_TICK_DAMAGE ) +
                               0.10 * p -> set_bonus.tier10_4pc_melee() );
  }

  virtual void player_buff()
  {
    paladin_attack_t::player_buff();
    player_multiplier *= p() -> buffs_censure -> stack();
  }

  virtual void travel( player_t* t, int travel_result, double travel_dmg=0 )
  {
    if ( result_is_hit( travel_result ) )
    {
      p() -> buffs_censure -> trigger();
      player_buff(); // update with new stack of the debuff
    }
    paladin_attack_t::travel( t, travel_result, travel_dmg );
  }

  virtual void last_tick()
  {
    paladin_t* p = player -> cast_paladin();
    paladin_attack_t::last_tick();
    p -> buffs_censure -> expire();
  }
};

struct seal_of_truth_proc_t : public paladin_attack_t
{
  seal_of_truth_proc_t( paladin_t* p )
    : paladin_attack_t( "seal_of_truth", 42463, p )
  {
    background  = true;
    proc        = true;
    may_miss    = false;
    may_dodge   = false;
    may_parry   = false;
    if ( p -> bugs )
      weapon_multiplier = 0.15; // files say 9% but in-game testing says 15%

    // For some reason, SotP is multiplicative with 4T10 for the procs but additive for the DoT
    base_multiplier *= ( 1.0 + 0.01 * p -> talents.seals_of_the_pure -> effect_base_value( 1 ) )
                       *  ( 1.0 + 0.10 * p -> set_bonus.tier10_4pc_melee() );
  }
  virtual void player_buff()
  {
    paladin_t* p = player -> cast_paladin();
    paladin_attack_t::player_buff();
    player_multiplier *= p -> buffs_censure -> stack() * 0.2;
  }
};

struct seal_of_truth_judgement_t : public paladin_attack_t
{
  seal_of_truth_judgement_t( paladin_t* p )
    : paladin_attack_t( "judgement_of_truth", 31804, p )
  {
    background   = true;
    may_parry    = false;
    may_dodge    = false;
    may_block    = false;
    trigger_seal = 1;

    base_crit       += p -> talents.arbiter_of_the_light -> mod_additive( P_CRIT );
    base_multiplier *= 1.0 + 0.10 * p -> set_bonus.tier10_4pc_melee()
                       + 0.01 * p -> talents.wrath_of_the_lightbringer->effect_base_value( 1 )
                       + p -> glyphs.judgement -> mod_additive( P_GENERIC );


    base_dd_min = base_dd_max    = 1.0;
    base_spell_power_multiplier  = direct_power_mod;
    base_attack_power_multiplier = extra_coeff();
    direct_power_mod             = 1.0;
  }

  virtual void player_buff()
  {
    paladin_t* p = player -> cast_paladin();
    paladin_attack_t::player_buff();
    player_multiplier *= 1.0 + p -> buffs_censure -> stack() * 0.10;
  }
};

// Seals of Command proc ====================================================

struct seals_of_command_proc_t : public paladin_attack_t
{
  seals_of_command_proc_t( paladin_t* p )
    : paladin_attack_t( "seals_of_command", 20424, p )
  {
    background  = true;
    proc        = true;
  }
};

// Judgement ================================================================

struct judgement_t : public paladin_attack_t
{
  action_t* seal_of_justice;
  action_t* seal_of_insight;
  action_t* seal_of_righteousness;
  action_t* seal_of_truth;

  judgement_t( paladin_t* p, const std::string& options_str )
    : paladin_attack_t( "judgement", "Judgement", p ),
      seal_of_justice( 0 ), seal_of_insight( 0 ),
      seal_of_righteousness( 0 ), seal_of_truth( 0 )
  {
    parse_options( NULL, options_str );

    trigger_dp = true;
    seal_of_justice       = new seal_of_justice_judgement_t      ( p );
    seal_of_insight       = new seal_of_insight_judgement_t      ( p );
    seal_of_righteousness = new seal_of_righteousness_judgement_t( p );
    seal_of_truth         = new seal_of_truth_judgement_t        ( p );
  }

  action_t* active_seal() SC_CONST
  {
    paladin_t* p = player -> cast_paladin();
    switch ( p -> active_seal )
    {
    case SEAL_OF_JUSTICE:
      return seal_of_justice;
    case SEAL_OF_INSIGHT:
      return seal_of_insight;
    case SEAL_OF_RIGHTEOUSNESS:
      return seal_of_righteousness;
    case SEAL_OF_TRUTH:
      return seal_of_truth;
    }
    return 0;
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    action_t* seal = active_seal();

    if ( ! seal )
      return;

    seal -> trigger_gcd = trigger_gcd;
    seal -> execute();

    if ( seal -> result_is_hit() )
    {

      if ( p -> talents.judgements_of_the_just -> rank() )
      {
        target -> debuffs.judgements_of_the_just -> trigger();
        target -> debuffs.judgements_of_the_just -> source = p;
      }

      p -> buffs_divine_purpose -> trigger();
      p -> buffs_judgements_of_the_pure -> trigger();
      p -> buffs_sacred_duty-> trigger();      
    }

    p -> buffs_judgements_of_the_bold -> trigger();
    p -> buffs_judgements_of_the_wise -> trigger();

    if ( p -> talents.communion -> rank() ) p -> trigger_replenishment();

    update_ready();

    p -> last_foreground_action = seal; // Necessary for DPET calculations.
  }

  virtual bool ready()
  {
    action_t* seal = active_seal();
    if( ! seal ) return false;
    return paladin_attack_t::ready();
  }
};

// Shield of Righteousness =================================================

struct shield_of_the_righteous_t : public paladin_attack_t
{
  shield_of_the_righteous_t( paladin_t* p, const std::string& options_str )
    : paladin_attack_t( "shield_of_the_righteous", "Shield of the Righteous", p )
  {
    check_talent( p -> talents.shield_of_the_righteous -> rank() );

    parse_options( NULL, options_str );

    may_parry = false;
    may_dodge = false;
    may_block = false;

    direct_power_mod = extra_coeff();
    base_multiplier *= 1.0 + p -> glyphs.shield_of_the_righteous -> mod_additive( P_GENERIC );
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    paladin_attack_t::execute();
    if ( p -> talents.holy_shield -> rank() )
      p -> buffs_holy_shield -> trigger();
    if ( result_is_hit() )
      p -> buffs_sacred_duty -> expire();
  }

  virtual void player_buff()
  {
    paladin_t* p = player -> cast_paladin();
    paladin_attack_t::player_buff();
    player_multiplier *= util_t::talent_rank( p -> holy_power_stacks(), 3, 1.0, 3.0, 6.0 );
    if ( p -> buffs_sacred_duty -> up() )
    {
      player_crit += 1.0;
    }
  }

  virtual bool ready()
  {
    paladin_t* p = player -> cast_paladin();

    if ( p -> main_hand_weapon.group() == WEAPON_2H )
      return false;

    return paladin_attack_t::ready();
  }
};

// Templar's Verdict ========================================================

struct templars_verdict_t : public paladin_attack_t
{
  templars_verdict_t( paladin_t* p, const std::string& options_str )
    : paladin_attack_t( "templars_verdict", "Templar's Verdict", p )
  {
    parse_options( NULL, options_str );

    trigger_seal      = true;
    trigger_dp        = true;

    base_crit       += p -> talents.arbiter_of_the_light -> mod_additive( P_CRIT );
    base_multiplier *= 1 + p -> talents.crusade -> mod_additive( P_GENERIC )
                       + p -> glyphs.templars_verdict -> mod_additive( P_GENERIC )
                       + ( p -> set_bonus.tier11_2pc_melee() ? 0.10 : 0.0 );
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    weapon_multiplier = util_t::talent_rank( p -> holy_power_stacks(), 3, 0.30, 0.90, 2.35 );
    paladin_attack_t::execute();
    if ( result_is_hit() )
    {
      trigger_hand_of_light( this );
    }
  }
};

// ==========================================================================
// Paladin Spells
// ==========================================================================

struct paladin_spell_t : public spell_t
{
  double jotp_haste;
  bool trigger_dp;

  paladin_spell_t( const char* n, paladin_t* p, const school_type s=SCHOOL_HOLY, int t=TREE_NONE )
    : spell_t( n, p, RESOURCE_MANA, s, t ), jotp_haste( 1.0 ), trigger_dp( false )
  {
    initialize_();
  }

  paladin_spell_t( const char* n, uint32_t id, paladin_t* p )
    : spell_t( n, id, p ), jotp_haste( 1.0 ), trigger_dp( false )
  {
    initialize_();
  }

  paladin_spell_t( const char *n, const char *sname, paladin_t* p )
    : spell_t( n, sname, p ), jotp_haste( 1.0 ), trigger_dp( false )
  {
    initialize_();
  }

  void initialize_()
  {
    base_multiplier *= 1.0 + 0.01 * p() -> talents.communion -> effect_base_value( 3 );

    if ( p() -> set_bonus.tier10_2pc_melee() )
      base_multiplier *= 1.05;

    if ( p() -> talents.judgements_of_the_pure -> rank() )
      jotp_haste = 1.0 / ( 1.0 + p() -> buffs_judgements_of_the_pure -> base_value( E_APPLY_AURA, A_HASTE_ALL ) );
  }

  paladin_t* p() SC_CONST
  {
    return static_cast<paladin_t*>( player );
  }

  virtual double haste() SC_CONST
  {
    paladin_t* p = player -> cast_paladin();
    double h = spell_t::haste();
    if ( p -> buffs_judgements_of_the_pure -> up() )
    {
      h *= jotp_haste;
    }
    return h;
  }

  virtual void player_buff()
  {
    paladin_t* p = player -> cast_paladin();

    spell_t::player_buff();

    player_multiplier *= 1.0 + p -> buffs_avenging_wrath -> value();

    if ( school == SCHOOL_HOLY )
    {
      player_multiplier *= 1.0 + p -> buffs_inquisition -> value();
    }
  }

  virtual void execute()
  {
    spell_t::execute();
    if ( result_is_hit() )
    {
      if ( trigger_dp )
      {
        paladin_t* p = player -> cast_paladin();
        p -> buffs_divine_purpose -> trigger();
      }
    }
  }

  virtual double cost() SC_CONST
  {
    paladin_t* p = player -> cast_paladin();

    if ( resource == RESOURCE_HOLY_POWER )
    {
      if ( p -> buffs_divine_purpose -> check() )
        return 0;

      return std::max( base_cost, p -> resource_current[ RESOURCE_HOLY_POWER ] );
    }

    return spell_t::cost();
   }

  virtual void consume_resource()
  {
    spell_t::consume_resource();

    paladin_t* p = player -> cast_paladin();

    p -> buffs_divine_purpose -> expire();
  }
};

// Avenging Wrath ===========================================================

struct avenging_wrath_t : public paladin_spell_t
{
  avenging_wrath_t( paladin_t* p, const std::string& options_str )
    : paladin_spell_t( "avenging_wrath", "Avenging Wrath", p )
  {
    parse_options( NULL, options_str );

    harmful = false;
    cooldown -> duration += p -> talents.sanctified_wrath -> mod_additive( P_COOLDOWN );
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    consume_resource();
    update_ready();
    p -> buffs_avenging_wrath -> trigger( 1, effect_base_value( 1 ) / 100.0 );
  }
};

// Consecration =============================================================

struct consecration_tick_t : public paladin_spell_t
{
  consecration_tick_t( paladin_t* p )
    : paladin_spell_t( "consecration", 81297, p )
  {
    aoe         = -1;
    dual        = true;
    direct_tick = true;
    background  = true;
    may_crit    = true;
    may_miss    = true;

    base_spell_power_multiplier  = direct_power_mod;
    base_attack_power_multiplier = extra_coeff();
    direct_power_mod             = 1.0;

    base_multiplier *= 1.0 + p -> talents.hallowed_ground -> mod_additive( P_GENERIC );
  }
};

struct consecration_t : public paladin_spell_t
{
  spell_t* tick_spell;

  consecration_t( paladin_t* p, const std::string& options_str )
    : paladin_spell_t( "consecration", "Consecration", p ), tick_spell( 0 )
  {
    parse_options( NULL, options_str );;

    may_miss       = false;
    num_ticks      = 10;
    base_tick_time = 1;
    base_cost     *= 1.0 + p -> talents.hallowed_ground -> mod_additive( P_RESOURCE_COST );

    if ( p -> glyphs.consecration -> ok() )
    {
      num_ticks = ( int ) floor( num_ticks * ( 1.0 + p -> glyphs.consecration -> mod_additive( P_DURATION ) ) );
      cooldown -> duration *= 1.0 + p -> glyphs.consecration -> mod_additive( P_COOLDOWN );
    }

    tick_spell = new consecration_tick_t( p );
  }

  virtual void tick()
  {
    if ( sim -> debug ) log_t::output( sim, "%s ticks (%d of %d)", name(), dot -> current_tick, dot -> num_ticks );
    tick_spell -> execute();
    stats -> add_tick( time_to_tick );
  }
};

// Divine Favor =============================================================

struct divine_favor_t : public paladin_spell_t
{
  divine_favor_t( paladin_t* p, const std::string& options_str )
    : paladin_spell_t( "divine_favor", "Divine Favor", p )
  {
    check_talent( p -> talents.divine_favor -> rank() );

    parse_options( NULL, options_str );

    harmful = false;
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    update_ready();
    p -> buffs_divine_favor -> trigger();
  }
};

// Divine Plea ==============================================================

struct divine_plea_t : public paladin_spell_t
{
  divine_plea_t( paladin_t* p, const std::string& options_str )
    : paladin_spell_t( "divine_plea", "Divine Plea", p )
  {
    parse_options( NULL, options_str );

    harmful = false;
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();

    paladin_spell_t::execute();

    p -> buffs_divine_plea -> trigger();

    int hopo = ( int ) p -> talents.shield_of_the_templar -> mod_additive( P_EFFECT_3 );
    if ( hopo )
    {
      p -> resource_gain( RESOURCE_HOLY_POWER, hopo,
                                p -> gains_hp_divine_plea );
    }
  }
};

// Exorcism ================================================================

struct exorcism_t : public paladin_spell_t
{
  int undead_demon;

  exorcism_t( paladin_t* p, const std::string& options_str )
    : paladin_spell_t( "exorcism", "Exorcism", p ), undead_demon( 0 )
  {
    option_t options[] =
    {
      { "undead_demon", OPT_BOOL, &undead_demon },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    base_attack_power_multiplier = 0.344;
    base_spell_power_multiplier  = 0.344;
    direct_power_mod             = 1.0;
    dot_behavior                 = DOT_REFRESH;
    hasted_ticks                 = false;
    may_crit                     = true;
    tick_may_crit                = true;
    trigger_dp                   = true;
    tick_power_mod               = 0.2/3; // glyph of exorcism is 20% of damage over three ticks

    if ( ! p -> glyphs.exorcism -> ok() )
      num_ticks = 0;
  }

  virtual double cost() SC_CONST
  {
    paladin_t* p = player -> cast_paladin();
    if ( p -> buffs_the_art_of_war -> check() ) return 0.0;
    return paladin_spell_t::cost();
  }

  virtual double execute_time() SC_CONST
  {
    paladin_t* p = player->cast_paladin();
    if ( p -> buffs_the_art_of_war -> check() ) return 0.0;
    return paladin_spell_t::execute_time();
  }

  virtual void player_buff()
  {
    paladin_spell_t::player_buff();
    paladin_t* p = player -> cast_paladin();

    player_multiplier *= 1.0 + p -> talents.blazing_light -> mod_additive( P_GENERIC )
                         + ( p -> buffs_the_art_of_war -> up() ? 1.0 : 0.0 );

    if ( target -> race == RACE_UNDEAD || target -> race == RACE_DEMON )
    {
      player_crit += 1.0;
    }
  }

  virtual double total_power() SC_CONST
  {
    return ( std::max )( total_spell_power(), total_attack_power() );
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    paladin_spell_t::execute();
    // FIXME: Should this be wrapped in a result_is_hit() ?
    switch ( p -> active_seal )
    {
    case SEAL_OF_JUSTICE:
      p -> active_seal_of_justice_proc       -> execute();
      break;
    case SEAL_OF_INSIGHT:
      p -> active_seal_of_insight_proc       -> execute();
      break;
    case SEAL_OF_RIGHTEOUSNESS:
      p -> active_seal_of_righteousness_proc -> execute();
      break;
    case SEAL_OF_TRUTH:
      if ( p -> buffs_censure -> stack() >= 1 ) p -> active_seal_of_truth_proc -> execute();
      break;
    default:
      ;
    }

    if ( p -> active_seal != SEAL_OF_INSIGHT && p -> talents.seals_of_command -> rank() )
    {
      p -> active_seals_of_command_proc -> execute();
    }
    p -> buffs_the_art_of_war -> expire();
  }

  virtual bool ready()
  {
    if ( undead_demon )
    {
      int target_race = target -> race;
      if ( target_race != RACE_UNDEAD &&
           target_race != RACE_DEMON  )
        return false;
    }

    return paladin_spell_t::ready();
  }
};

// Guardian of the Ancient Kings ============================================

struct guardian_of_ancient_kings_t : public paladin_spell_t
{
  guardian_of_ancient_kings_t( paladin_t* p, const std::string& options_str )
    : paladin_spell_t( "guardian_of_ancient_kings", 86150, p )
  {
    parse_options( NULL, options_str );
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    update_ready();
    if ( p -> primary_tree() == TREE_RETRIBUTION )
      p -> summon_pet( "guardian_of_ancient_kings", p -> spells.guardian_of_ancient_kings_ret -> duration() );
  }
};

// Holy Shock ==============================================================

// TODO: fix the fugly hack
struct holy_shock_t : public paladin_spell_t
{
  holy_shock_t( paladin_t* p, const std::string& options_str )
    : paladin_spell_t( "holy_shock", "Holy Shock", p )
  {
    parse_options( NULL, options_str );

    // hack! spell 20473 has the cooldown/cost/etc stuff, but the actual spell cast
    // to do damage is 25912
    parse_effect_data( 25912, 1 );

    base_multiplier *= 1.0 + p -> talents.blazing_light -> mod_additive( P_GENERIC )
                       + p -> talents.crusade -> mod_additive( P_GENERIC ); // TODO how do they stack?
    base_crit       += p -> glyphs.holy_shock -> mod_additive( P_CRIT );
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    paladin_spell_t::execute();
    if ( result_is_hit() )
    {
      p -> resource_gain( RESOURCE_HOLY_POWER, 1,
                                p -> gains_hp_holy_shock );
    }
  }
};

// Holy Wrath ==============================================================

struct holy_wrath_t : public paladin_spell_t
{
  holy_wrath_t( paladin_t* p, const std::string& options_str )
    : paladin_spell_t( "holy_wrath", "Holy Wrath", p )
  {
    parse_options( NULL, options_str );;

    // aoe = -1; FIXME disabled until we have meteor support
    may_crit   = true;
    trigger_dp = true;

    direct_power_mod = 0.61;

    base_crit += p -> talents.wrath_of_the_lightbringer -> mod_additive( P_CRIT );
  }
};

// Inquisition ==============================================================

struct inquisition_t : public paladin_spell_t
{
  double base_duration;
  inquisition_t( paladin_t* p, const std::string& options_str )
    : paladin_spell_t( "inquisition", "Inquisition", p ), base_duration( 0 )
  {
    check_min_level( 81 );

    parse_options( NULL, options_str );

    harmful = false;
    base_duration = duration() * ( 1.0 + p -> talents.inquiry_of_faith -> mod_additive( P_DURATION ) );
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();

    paladin_spell_t::execute();

    p -> buffs_inquisition -> buff_duration = base_duration * p -> holy_power_stacks();
    if ( p -> set_bonus.tier11_4pc_melee() )
      p -> buffs_inquisition -> buff_duration += base_duration;
    p -> buffs_inquisition -> trigger( 1, base_value() );
    if ( p -> talents.holy_shield -> rank() )
      p -> buffs_holy_shield -> trigger();
    p -> buffs_divine_purpose -> trigger();
  }
};

// Zealotry =================================================================

struct zealotry_t : public paladin_spell_t
{
  zealotry_t( paladin_t* p, const std::string& options_str )
    : paladin_spell_t( "zealotry", "Zealotry", p )
  {
    check_talent( p -> talents.zealotry -> rank() );

    parse_options( NULL, options_str );

    harmful = false;
  }

  virtual void execute()
  {
    paladin_t* p = player -> cast_paladin();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    update_ready();
    p -> buffs_zealotry -> trigger();
  }

  virtual void consume_resource()
  {
  }
};


// =========================================================================
// Paladin Heals
// =========================================================================

struct paladin_heal_t : public heal_t
{
  void _init_paladin_heal_t()
  {
    may_crit          = true;
    tick_may_crit     = true;

    dot_behavior      = DOT_REFRESH;
    weapon_multiplier = 0.0;
  }
  paladin_heal_t( const char* n, player_t* player, const char* sname, int t = TREE_NONE ) :
      heal_t( n, player, sname, t )
  {
    _init_paladin_heal_t();
  }

  paladin_heal_t( const char* n, player_t* player, const uint32_t id, int t = TREE_NONE ) :
      heal_t( n, player, id, t )
  {
    _init_paladin_heal_t();
  }

  virtual void player_buff()
  {
    heal_t::player_buff();
  }

  virtual double cost() SC_CONST
  {
    paladin_t* p = player -> cast_paladin();

    if ( resource == RESOURCE_HOLY_POWER )
    {
      if ( p -> buffs_divine_purpose -> check() )
        return 0;

      return std::max( base_cost, p -> resource_current[ RESOURCE_HOLY_POWER ] );
    }

    return heal_t::cost();
   }

  virtual void consume_resource()
  {
    heal_t::consume_resource();

    paladin_t* p = player -> cast_paladin();

    p -> buffs_divine_purpose -> expire();
  }
};


// Holy Light Spell

struct holy_light_t : public paladin_heal_t
{
  holy_light_t( paladin_t* p, const std::string& options_str ) :
    paladin_heal_t( "holy_light", p, "Holy Light" )
  {
    parse_options( NULL, options_str );
  }
};

// Word of Glory Spell

struct word_of_glory_t : public paladin_heal_t
{
  word_of_glory_t( paladin_t* p, const std::string& options_str ) :
    paladin_heal_t( "word_of_glory", p, "Word of Glory" )
  {
    parse_options( NULL, options_str );

    base_attack_power_multiplier = 1.0;
    base_spell_power_multiplier  = 0.0;

    // !Glyph deactivates the hot.
  }

  virtual void player_buff()
  {
    paladin_heal_t::player_buff();

    paladin_t* p = player -> cast_paladin();

    player_multiplier *= p -> holy_power_stacks();
  }
};

} // ANONYMOUS NAMESPACE ===================================================

// ==========================================================================
// Paladin Character Definition
// ==========================================================================

// paladin_t::create_action ==================================================

action_t* paladin_t::create_action( const std::string& name, const std::string& options_str )
{
  if ( name == "auto_attack"               ) return new auto_attack_t              ( this, options_str );
  if ( name == "avengers_shield"           ) return new avengers_shield_t          ( this, options_str );
  if ( name == "avenging_wrath"            ) return new avenging_wrath_t           ( this, options_str );
  if ( name == "consecration"              ) return new consecration_t             ( this, options_str );
  if ( name == "crusader_strike"           ) return new crusader_strike_t          ( this, options_str );
  if ( name == "divine_favor"              ) return new divine_favor_t             ( this, options_str );
  if ( name == "divine_plea"               ) return new divine_plea_t              ( this, options_str );
  if ( name == "divine_storm"              ) return new divine_storm_t             ( this, options_str );
  if ( name == "exorcism"                  ) return new exorcism_t                 ( this, options_str );
  if ( name == "hammer_of_justice"         ) return new hammer_of_justice_t        ( this, options_str );
  if ( name == "hammer_of_wrath"           ) return new hammer_of_wrath_t          ( this, options_str );
  if ( name == "hammer_of_the_righteous"   ) return new hammer_of_the_righteous_t  ( this, options_str );
  if ( name == "holy_shock"                ) return new holy_shock_t               ( this, options_str );
  if ( name == "holy_wrath"                ) return new holy_wrath_t               ( this, options_str );
  if ( name == "guardian_of_ancient_kings" ) return new guardian_of_ancient_kings_t( this, options_str );
  if ( name == "inquisition"               ) return new inquisition_t              ( this, options_str );
  if ( name == "judgement"                 ) return new judgement_t                ( this, options_str );
  if ( name == "shield_of_the_righteous"   ) return new shield_of_the_righteous_t  ( this, options_str );
  if ( name == "templars_verdict"          ) return new templars_verdict_t         ( this, options_str );
  if ( name == "zealotry"                  ) return new zealotry_t                 ( this, options_str );

  if ( name == "seal_of_justice"           ) return new paladin_seal_t( this, "seal_of_justice",       SEAL_OF_JUSTICE,       options_str );
  if ( name == "seal_of_insight"           ) return new paladin_seal_t( this, "seal_of_insight",       SEAL_OF_INSIGHT,       options_str );
  if ( name == "seal_of_righteousness"     ) return new paladin_seal_t( this, "seal_of_righteousness", SEAL_OF_RIGHTEOUSNESS, options_str );
  if ( name == "seal_of_truth"             ) return new paladin_seal_t( this, "seal_of_truth",         SEAL_OF_TRUTH,         options_str );

  //if ( name == "aura_mastery"            ) return new aura_mastery_t           ( this, options_str );
  //if ( name == "blessings"               ) return new blessings_t              ( this, options_str );
  //if ( name == "concentration_aura"      ) return new concentration_aura_t     ( this, options_str );
  //if ( name == "devotion_aura"           ) return new devotion_aura_t          ( this, options_str );
  //if ( name == "retribution_aura"        ) return new retribution_aura_t       ( this, options_str );

  if ( name == "holy_light"                ) return new holy_light_t               ( this, options_str );
  if ( name == "word_of_glory"             ) return new word_of_glory_t            ( this, options_str );

  return player_t::create_action( name, options_str );
}

// paladin_t::init_base =====================================================

void paladin_t::init_base()
{
  player_t::init_base();

  initial_attack_power_per_strength = 2.0;
  initial_spell_power_per_intellect = 1.0;

  base_spell_power  = 0;
  base_attack_power = ( level * 3 ) - 20;

  resource_base[ RESOURCE_HOLY_POWER ] = 3;

  // FIXME! Level-specific!
  base_miss    = 0.05;
  base_parry   = 0.05;
  base_block   = 0.05;
  initial_armor_multiplier *= 1.0 + 0.1 * talents.toughness -> effect_base_value( 1 );

  diminished_kfactor    = 0.009560;
  diminished_dodge_capi = 0.01523660;
  diminished_parry_capi = 0.01523660;

  mana_per_intellect = 15;

  switch ( primary_tree() )
  {
  case TREE_HOLY:
    base_attack_hit += 0; // TODO spirit -> hit talents.enlightened_judgements
    base_spell_hit  += 0; // TODO spirit -> hit talents.enlightened_judgements
    mana_regen_while_casting = 0.50;
    break;

  case TREE_PROTECTION:
    attribute_multiplier_initial[ ATTR_STAMINA   ] *= 1.0 + passives.touched_by_the_light -> base_value( E_APPLY_AURA, A_MOD_TOTAL_STAT_PERCENTAGE );
    // effect is actually on JotW since there's not room for more effects on TbtL
    base_spell_hit += passives.judgements_of_the_wise -> base_value( E_APPLY_AURA, A_MOD_SPELL_HIT_CHANCE );
    break;

  case TREE_RETRIBUTION:
    base_spell_hit += passives.sheath_of_light -> base_value( E_APPLY_AURA, A_MOD_SPELL_HIT_CHANCE );
    break;
  default:
    break;
  }
}

// paladin_t::reset =========================================================

void paladin_t::reset()
{
  player_t::reset();

  active_seal = SEAL_NONE;
}

// paladin_t::init_gains ====================================================

void paladin_t::init_gains()
{
  player_t::init_gains();

  gains_divine_plea            = get_gain( "divine_plea"            );
  gains_judgements_of_the_wise = get_gain( "judgements_of_the_wise" );
  gains_judgements_of_the_bold = get_gain( "judgements_of_the_bold" );
  gains_sanctuary              = get_gain( "sanctuary"              );
  gains_seal_of_command_glyph  = get_gain( "seal_of_command_glyph"  );
  gains_seal_of_insight        = get_gain( "seal_of_insight"        );

  // Holy Power
  gains_hp_crusader_strike          = get_gain( "holy_power_crusader_strike" );
  gains_hp_holy_shock               = get_gain( "holy_power_holy_shock" );
  gains_hp_blessed_life             = get_gain( "holy_power_blessed_life" );
  gains_hp_tower_of_radiance        = get_gain( "holy_power_tower_of_radiance" );
  gains_hp_hammer_of_the_righteous  = get_gain( "holy_power_hammer_of_the_righteous" );
  gains_hp_divine_plea              = get_gain( "holy_power_divine_plea" );
  gains_hp_pursuit_of_justice       = get_gain( "holy_power_pursuit_of_justice" );
  gains_hp_zealotry                 = get_gain( "holy_power_zealotry" );
  gains_hp_divine_storm             = get_gain( "holy_power_divine_storm" );
}

// paladin_t::init_procs ====================================================

void paladin_t::init_procs()
{
  player_t::init_procs();

  procs_parry_haste = get_proc( "parry_haste" );
}

// paladin_t::init_scaling ==================================================

void paladin_t::init_scaling()
{
  player_t::init_scaling();

  int tree = primary_tree();

  // Technically prot and ret scale with int and sp too, but it's so minor it's not worth the sim time.
  scales_with[ STAT_INTELLECT   ] = tree == TREE_HOLY;
  scales_with[ STAT_SPIRIT      ] = tree == TREE_HOLY;
  scales_with[ STAT_SPELL_POWER ] = tree == TREE_HOLY;

  if ( primary_role() == ROLE_TANK )
  {
    scales_with[ STAT_PARRY_RATING ] = 1;
    scales_with[ STAT_BLOCK_RATING ] = 1;
  }
}

// paladin_t::decode_set ====================================================

int paladin_t::decode_set( item_t& item )
{
  if ( item.slot != SLOT_HEAD      &&
       item.slot != SLOT_SHOULDERS &&
       item.slot != SLOT_CHEST     &&
       item.slot != SLOT_HANDS     &&
       item.slot != SLOT_LEGS      )
  {
    return SET_NONE;
  }

  const char* s = item.name();

  if ( item.slot == SLOT_HANDS && strstr( s, "gladiators_scaled_gauntlets" ) && item.ilevel > 140 )
  {
    ret_pvp_gloves = 1;
  }

  bool is_melee = ( strstr( s, "helm"           ) ||
                    strstr( s, "shoulderplates" ) ||
                    strstr( s, "battleplate"    ) ||
                    strstr( s, "chestpiece"     ) ||
                    strstr( s, "legplates"      ) ||
                    strstr( s, "gauntlets"      ) );

  bool is_tank = ( strstr( s, "faceguard"      ) ||
                   strstr( s, "shoulderguards" ) ||
                   strstr( s, "breastplate"    ) ||
                   strstr( s, "legguards"      ) ||
                   strstr( s, "handguards"     ) );

  if ( strstr( s, "lightsworn" ) )
  {
    if ( is_melee  ) return SET_T10_MELEE;
    if ( is_tank   ) return SET_T10_TANK;
  }
  if ( strstr( s, "reinforced_sapphirium" ) )
  {
    bool is_melee = ( strstr( s, "helmet"        ) ||
                      strstr( s, "pauldrons"     ) ||
                      strstr( s, "battleplate"   ) ||
                      strstr( s, "legplates"     ) ||
                      strstr( s, "gauntlets"     ) );

    bool is_tank = ( strstr( s, "faceguard"      ) ||
                     strstr( s, "shoulderguards" ) ||
                     strstr( s, "chestguard"     ) ||
                     strstr( s, "legguards"      ) ||
                     strstr( s, "handguards"     ) );

    if ( is_melee  ) return SET_T11_MELEE;
    if ( is_tank   ) return SET_T11_TANK;
  }

  return SET_NONE;
}

// paladin_t::init_buffs ====================================================

void paladin_t::init_buffs()
{
  player_t::init_buffs();

  // buff_t( player, name, max_stack, duration, cooldown, proc_chance, quiet )
  buffs_ancient_power          = new buff_t( this, 86700, "ancient_power" );
  buffs_avenging_wrath         = new buff_t( this, 31884, "avenging_wrath",  1, 0 ); // Let the ability handle the CD
  buffs_censure                = new buff_t( this, 31803, "censure" );
  buffs_divine_favor           = new buff_t( this, "divine_favor",           1, spells.divine_favor -> duration() + glyphs.divine_favor -> mod_additive( P_DURATION ) );
  buffs_divine_plea            = new buff_t( this, 54428, "divine_plea", 1, 0 ); // Let the ability handle the CD
  buffs_divine_purpose         = new buff_t( this, 90174, "divine_purpose", 0.01 * talents.divine_purpose -> effect_base_value( 1 ) );
  buffs_holy_shield            = new buff_t( this, 87342, "holy_shield" );
  buffs_inquisition            = new buff_t( this, 84963, "inquisition" );
  buffs_judgements_of_the_bold = new buff_t( this, 89906, "judgements_of_the_bold", ( primary_tree() == TREE_RETRIBUTION ? 1 : 0 ) );
  buffs_judgements_of_the_pure = new buff_t( this, talents.judgements_of_the_pure -> effect_trigger_spell( 1 ), "judgements_of_the_pure", talents.judgements_of_the_pure -> proc_chance() );
  buffs_judgements_of_the_wise = new buff_t( this, 31930, "judgements_of_the_wise", ( primary_tree() == TREE_PROTECTION ? 1 : 0 ) );
  buffs_reckoning              = new buff_t( this, talents.reckoning -> effect_trigger_spell( 1 ), "reckoning", talents.reckoning -> proc_chance() );
  buffs_sacred_duty            = new buff_t( this, 85433, "sacred_duty", talents.sacred_duty -> proc_chance() );
  buffs_the_art_of_war         = new buff_t( this, talents.the_art_of_war -> effect_trigger_spell( 1 ), "the_art_of_war",  talents.the_art_of_war -> proc_chance() );
  buffs_zealotry               = new buff_t( this, talents.zealotry -> spell_id(), "zealotry", 1 );
}

// paladin_t::init_actions ==================================================

void paladin_t::init_actions()
{
  if ( main_hand_weapon.type == WEAPON_NONE )
  {
    sim -> errorf( "Player %s has no weapon equipped at the Main-Hand slot.", name() );
    quiet = true;
    return;
  }

  active_hand_of_light_proc         = new hand_of_light_proc_t        ( this );
  active_seals_of_command_proc      = new seals_of_command_proc_t     ( this );
  active_seal_of_justice_proc       = new seal_of_justice_proc_t      ( this );
  active_seal_of_insight_proc       = new seal_of_insight_proc_t      ( this );
  active_seal_of_righteousness_proc = new seal_of_righteousness_proc_t( this );
  active_seal_of_truth_proc         = new seal_of_truth_proc_t        ( this );
  active_seal_of_truth_dot          = new seal_of_truth_dot_t         ( this );
  ancient_fury_explosion            = new ancient_fury_t              ( this );

  if ( action_list_str.empty() )
  {
    switch ( primary_tree() )
    {
    case TREE_RETRIBUTION:
    {
      if ( level > 80 )
      {
        action_list_str += "/flask,type=titanic_strength/food,type=beer_basted_crocolisk";
      }
      else
      {
        action_list_str += "/flask,type=endless_rage/food,type=dragonfin_filet";
      }
      action_list_str += "/seal_of_truth";
      action_list_str += "/snapshot_stats";
      // TODO: action_list_str += "/rebuke";

      if ( level > 80 )
      {
        action_list_str += "/golemblood_potion,if=!in_combat|buff.bloodlust.react|target.time_to_die<=40";
      }
      else
      {
        action_list_str += "/speed_potion,if=!in_combat|buff.bloodlust.react|target.time_to_die<=60";
      }

      // This should<tm> get Censure up before the auto attack lands
      action_list_str += "/auto_attack/judgement,if=buff.judgements_of_the_pure.down";
      int num_items = ( int ) items.size();
      for ( int i=0; i < num_items; i++ )
      {
        if ( items[ i ].use.active() )
        {
          action_list_str += "/use_item,name=";
          action_list_str += items[ i ].name();
        }
      }
      std::string hp_proc_str = "divine_purpose";
      if ( race == RACE_BLOOD_ELF ) action_list_str += "/arcane_torrent";
      if ( level >= 85 )
        action_list_str += "/guardian_of_ancient_kings";
      action_list_str += "/avenging_wrath,if=buff.zealotry.down";
      action_list_str += "/zealotry,if=buff.avenging_wrath.down";
      if ( level >= 81 )
        action_list_str += "/inquisition,if=(buff.inquisition.down|buff.inquisition.remains<5)&(holy_power=3|buff."+hp_proc_str+".react)";
      // CS before TV if <3 power, even with HoL/DP up
      action_list_str += "/templars_verdict,if=holy_power=3";
      action_list_str += "/crusader_strike,if=buff."+hp_proc_str+".react&(buff."+hp_proc_str+".remains>2)&holy_power<3";
      action_list_str += "/templars_verdict,if=buff."+hp_proc_str+".react";
      action_list_str += "/crusader_strike";
      action_list_str += "/hammer_of_wrath";
      action_list_str += "/exorcism,if=buff.the_art_of_war.react";
      action_list_str += "/judgement,if=buff.judgements_of_the_pure.remains<2";
      // Don't delay CS too much
      action_list_str += "/wait,sec=0.1,if=cooldown.crusader_strike.remains<0.5";
      action_list_str += "/judgement";
      action_list_str += "/holy_wrath";
      action_list_str += "/consecration";
      action_list_str += "/divine_plea";
    }
    break;
    case TREE_PROTECTION:
    {
      if ( level > 80 )
      {
        action_list_str = "flask,type=steelskin/food,type=beer_basted_crocolisk";
        action_list_str += "/earthen_potion,if=!in_combat|buff.bloodlust.react|target.time_to_die<=60";
      }
      else
      {
        action_list_str = "flask,type=stoneblood/food,type=dragonfin_filet";
        action_list_str += "/indestructible_potion,if=!in_combat|buff.bloodlust.react|target.time_to_die<=60";
      }
      action_list_str += "/seal_of_truth";
      action_list_str += "/snapshot_stats";
      action_list_str += "/auto_attack";
      if ( race == RACE_BLOOD_ELF ) action_list_str += "/arcane_torrent";
      action_list_str += "/avenging_wrath";
      action_list_str += "/shield_of_the_righteous,if=holy_power=3";
      action_list_str += "/crusader_strike";
      action_list_str += "/judgement";
      action_list_str += "/avengers_shield";
      action_list_str += "/holy_wrath";
      action_list_str += "/consecration";
      action_list_str += "/divine_plea";
    }
    break;
    case TREE_HOLY:
    {
      if ( level > 80 )
      {
        action_list_str = "flask,type=steelskin/food,type=beer_basted_crocolisk";
        action_list_str += "/earthen_potion,if=!in_combat|buff.bloodlust.react|target.time_to_die<=60";
      }
      else
      {
        action_list_str = "flask,type=stoneblood/food,type=dragonfin_filet";
        action_list_str += "/indestructible_potion,if=!in_combat|buff.bloodlust.react|target.time_to_die<=60";
      }
      action_list_str += "/seal_of_truth";
      action_list_str += "/snapshot_stats";
      action_list_str += "/auto_attack";
      if ( race == RACE_BLOOD_ELF ) action_list_str += "/arcane_torrent";
      action_list_str += "/avenging_wrath";
      action_list_str += "/judgement";
      action_list_str += "/holy_wrath";
      action_list_str += "/holy_shock";
      action_list_str += "/consecration";
      action_list_str += "/divine_plea";
    }
    break;
    default:
      if ( ! quiet ) abort();
      break;
    }
    action_list_default = 1;
  }

  player_t::init_actions();
}

void paladin_t::init_talents()
{
  // Holy
  talents.arbiter_of_the_light   = find_talent( "Arbiter of the Light" );
  talents.judgements_of_the_pure = find_talent( "Judgements of the Pure" );
  talents.blazing_light          = find_talent( "Blazing Light" );
  talents.divine_favor           = find_talent( "Divine Favor" );
  // Prot
  talents.seals_of_the_pure         = find_talent( "Seals of the Pure" );
  talents.judgements_of_the_just    = find_talent( "Judgements of the Just" );
  talents.improved_hammer_of_justice= find_talent( "Improved Hammer of Justice" );
  talents.hallowed_ground           = find_talent( "Hallowed Ground" );
  talents.sanctuary                 = find_talent( "Sanctuary" );
  talents.hammer_of_the_righteous   = find_talent( "Hammer of the Righteous" );
  talents.wrath_of_the_lightbringer = find_talent( "Wrath of the Lightbringer" );
  talents.shield_of_the_righteous   = find_talent( "Shield of the Righteous" );
  talents.grand_crusader            = find_talent( "Grand Crusader" );
  talents.holy_shield               = find_talent( "Holy Shield" );
  talents.sacred_duty               = find_talent( "Sacred Duty" );
  talents.shield_of_the_templar     = find_talent( "Shield of the Templar" );
  talents.reckoning                 = find_talent( "Reckoning" );
  talents.toughness                 = find_talent( "Toughness" );

  // Ret
  talents.crusade            = find_talent( "Crusade" );
  talents.rule_of_law        = find_talent( "Rule of Law" );
  talents.communion          = find_talent( "Communion" );
  talents.the_art_of_war     = find_talent( "The Art of War" );
  talents.divine_storm       = find_talent( "Divine Storm" );
  talents.sanctity_of_battle = find_talent( "Sanctity of Battle" );
  talents.seals_of_command   = find_talent( "Seals of Command" );
  talents.divine_purpose     = find_talent( "Divine Purpose" );
  talents.sanctified_wrath   = find_talent( "Sanctified Wrath" );
  talents.inquiry_of_faith   = find_talent( "Inquiry of Faith" );
  talents.zealotry           = find_talent( "Zealotry" );

  // NYI
  talents.ardent_defender = 0 ;
  talents.aura_mastery = 0;
  talents.blessed_life = 0;
  talents.enlightened_judgements = 0;
  talents.eternal_glory = 0;
  talents.eye_for_an_eye = 0;
  talents.guarded_by_the_light = 0;
  talents.improved_judgement = 0;
  talents.long_arm_of_the_law = 0;
  talents.vindication = 0;

  player_t::init_talents();
}

void paladin_t::init_spells()
{
  player_t::init_spells();

  // Spells 
  spells.divine_favor                  = new active_spell_t( this, "divine_favor", "Divine Favor", talents.divine_favor );
  spells.guardian_of_ancient_kings_ret = new active_spell_t( this, "guardian_of_ancient_kings", 86698 );

  // Passives
  passives.divine_bulwark         = new mastery_t( this, "divine_bulwark", "Divine Bulwark", TREE_PROTECTION );
  passives.hand_of_light          = new mastery_t( this, "hand_of_light", "Hand of Light", TREE_RETRIBUTION );
  passives.judgements_of_the_bold = new passive_spell_t( this, "judgements_of_the_bold", "Judgements of the Bold" );
  passives.judgements_of_the_wise = new passive_spell_t( this, "judgements_of_the_wise", "Judgements of the Wise" );
  passives.plate_specialization   = new passive_spell_t( this, "plate_specialization", 86525 );
  passives.sheath_of_light        = new passive_spell_t( this, "sheath_of_light", "Sheath of Light" );
  passives.touched_by_the_light   = new passive_spell_t( this, "touched_by_the_light", "Touched by the Light" );
  passives.two_handed_weapon_spec = new passive_spell_t( this, "two_handed_weapon_specialization", "Two-Handed Weapon Specialization" );
  passives.vengeance              = new passive_spell_t( this, "vengeance", "Vengeance" );
  if ( passives.vengeance -> ok() )
    vengeance_enabled = true;

  // Glyphs
  glyphs.ascetic_crusader         = find_glyph( "Glyph of the Ascetic Crusader" );
  glyphs.consecration             = find_glyph( "Glyph of Consecration" );
  glyphs.crusader_strike          = find_glyph( "Glyph of Crusader Strike" );
  glyphs.divine_favor             = find_glyph( "Glyph of Divine Favor" );
  glyphs.divine_plea              = find_glyph( "Glyph of Divine Plea" );
  glyphs.exorcism                 = find_glyph( "Glyph of Exorcism" );
  glyphs.focused_shield           = find_glyph( "Glyph of Focused Shield" );
  glyphs.hammer_of_the_righteous  = find_glyph( "Glyph of Hammer of the Righteous" );
  glyphs.hammer_of_wrath          = find_glyph( "Glyph of Hammer of Wrath" );
  glyphs.holy_shock               = find_glyph( "Glyph of Holy Shock" );
  glyphs.judgement                = find_glyph( "Glyph of Judgement" );
  glyphs.seal_of_truth            = find_glyph( "Glyph of Seal of Truth" );
  glyphs.shield_of_the_righteous  = find_glyph( "Glyph of Shield of the Righteous" );
  glyphs.templars_verdict         = find_glyph( "Glyph of Templar's Verdict" );

  // Tier Bonuses
  static uint32_t set_bonuses[N_TIER][N_TIER_BONUS] =
  {
    //  C2P    C4P    M2P    M4P    T2P    T4P    H2P    H4P
    {     0,     0, 70765, 70762, 70758, 70761,     0,     0 }, // Tier10
    {     0,     0, 90298, 90299, 90301, 90306,     0,     0 }, // Tier11
    {     0,     0,     0,     0,     0,     0,     0,     0 },
  };

  sets = new set_bonus_array_t( this, set_bonuses );
}

// paladin_t::primary_role ===============================================

int paladin_t::primary_role() SC_CONST
{
  if ( player_t::primary_role() == ROLE_DPS || player_t::primary_role() == ROLE_HYBRID )
    return ROLE_HYBRID;

  if ( player_t::primary_role() == ROLE_TANK  )
    return ROLE_TANK;

  if ( player_t::primary_role() == ROLE_HEAL )
    return ROLE_HEAL;

  if ( primary_tree() == TREE_PROTECTION )
    return ROLE_TANK;

  return ROLE_HYBRID;
}

// paladin_t::composite_attack_expertise =================================

double paladin_t::composite_attack_expertise() SC_CONST
{
  double m = player_t::composite_attack_expertise();
  if ( active_seal == SEAL_OF_TRUTH && glyphs.seal_of_truth -> ok() )
  {
    m += glyphs.seal_of_truth -> mod_additive( P_EFFECT_2 ) / 100.0;
  }
  return m;
}

// paladin_t::composite_attribute_multiplier =================================

double paladin_t::composite_attribute_multiplier( int attr ) SC_CONST
{
  double m = player_t::composite_attribute_multiplier( attr );
  if ( attr == ATTR_STRENGTH && buffs_ancient_power -> check() )
  {
    m *= 1.0 + 0.01 * buffs_ancient_power -> stack();
  }
  return m;
}

// paladin_t::composite_spell_power ==========================================

double paladin_t::composite_spell_power( const school_type school ) SC_CONST
{
  double sp = player_t::composite_spell_power( school );
  switch ( primary_tree() )
  {
  case TREE_PROTECTION:
    sp += strength() * passives.touched_by_the_light -> base_value( E_APPLY_AURA, A_MOD_SPELL_DAMAGE_OF_STAT_PERCENT );
    break;
  case TREE_RETRIBUTION:
    sp += composite_attack_power_multiplier() * composite_attack_power() * passives.sheath_of_light -> base_value( E_APPLY_AURA, A_MOD_SPELL_DAMAGE_OF_ATTACK_POWER );
    break;
  default:
    break;
  }
  return sp;
}

// paladin_t::composite_tank_block ===========================================

double paladin_t::composite_tank_block() SC_CONST
{
  double b = player_t::composite_tank_block();
  b += get_divine_bulwark();
  return b;
}

// paladin_t::composite_tank_crit ==========================================

double paladin_t::composite_tank_crit( const school_type school ) SC_CONST
{
  double c = player_t::composite_tank_crit( school );

  if ( school == SCHOOL_PHYSICAL && talents.sanctuary -> rank() )
    c += talents.sanctuary -> effect_base_value( 3 ) / 100.0;

  return c;
}

// paladin_t::matching_gear_multiplier =====================================

double paladin_t::matching_gear_multiplier( const attribute_type attr ) SC_CONST
{
  double mult = 0.01 * passives.plate_specialization -> effect_base_value( 1 );
  switch ( primary_tree() )
  {
  case TREE_PROTECTION:
    if ( attr == ATTR_STAMINA )
      return mult;
    break;
  case TREE_RETRIBUTION:
    if ( attr == ATTR_STRENGTH )
      return mult;
    break;
  case TREE_HOLY:
    if ( attr == ATTR_INTELLECT )
      return mult;
    break;
  default:
    break;
  }
  return 0.0;
}

// paladin_t::regen  ========================================================

void paladin_t::regen( double periodicity )
{
  player_t::regen( periodicity );

  if ( buffs_divine_plea -> up() )
  {
    double tick_pct = ( buffs_divine_plea -> effect_base_value( 1 ) + glyphs.divine_plea -> mod_additive( P_EFFECT_1 ) ) * 0.01;
    double tick_amount = resource_max[ RESOURCE_MANA ] * tick_pct;
    double amount = periodicity * tick_amount / 3;
    resource_gain( RESOURCE_MANA, amount, gains_divine_plea );
  }
  if ( buffs_judgements_of_the_wise -> up() )
  {
    double tot_amount = resource_base[ RESOURCE_MANA ] * buffs_judgements_of_the_wise->effect_base_value( 1 ) * 0.01;
    double amount = periodicity * tot_amount / buffs_judgements_of_the_wise -> buff_duration;
    resource_gain( RESOURCE_MANA, amount, gains_judgements_of_the_wise );
  }
  if ( buffs_judgements_of_the_bold -> up() )
  {
    double tot_amount = resource_base[ RESOURCE_MANA ] * buffs_judgements_of_the_bold->effect_base_value( 1 ) * 0.01;
    double amount = periodicity * tot_amount / buffs_judgements_of_the_bold -> buff_duration;
    resource_gain( RESOURCE_MANA, amount, gains_judgements_of_the_bold );
  }
}

// paladin_t::assess_damage ==================================================

double paladin_t::assess_damage( double            amount,
                                 const school_type school,
                                 int               dmg_type,
                                 int               result,
                                 action_t*         action )
{

  if ( talents.sanctuary -> rank() )
  {
    amount *= 1.0 - talents.sanctuary -> effect_base_value( 1 ) / 100.0;

    if ( result == RESULT_DODGE || result == RESULT_BLOCK )
    {
      resource_gain( RESOURCE_MANA, resource_max[ RESOURCE_MANA ] * 0.02, gains_sanctuary );
    }
  }
  if ( result == RESULT_BLOCK )
  {
    buffs_reckoning -> trigger();
  }
  if ( result == RESULT_PARRY )
  {
    if ( main_hand_attack && main_hand_attack -> execute_event )
    {
      double swing_time = main_hand_attack -> time_to_execute;
      double max_reschedule = ( main_hand_attack -> execute_event -> occurs() - 0.20 * swing_time ) - sim -> current_time;

      if ( max_reschedule > 0 )
      {
        main_hand_attack -> reschedule_execute( std::min( ( 0.40 * swing_time ), max_reschedule ) );
        procs_parry_haste -> occur();
      }
    }
  }

  return player_t::assess_damage( amount, school, dmg_type, result, action );
}

// paladin_t::get_cooldown ===================================================

cooldown_t* paladin_t::get_cooldown( const std::string& name )
{
  if ( name == "hammer_of_the_righteous" ) return player_t::get_cooldown( "crusader_strike" );
  if ( name == "divine_storm"            ) return player_t::get_cooldown( "crusader_strike" );

  return player_t::get_cooldown( name );
}

// paladin_t::create_pet =====================================================

pet_t* paladin_t::create_pet( const std::string& pet_name,
                              const std::string& pet_type )
{
  pet_t* p = find_pet( pet_name );
  if ( p ) return p;

  if ( pet_name == "guardian_of_ancient_kings_ret" )
  {
    return new guardian_of_ancient_kings_ret_t( sim, this );
  }
  return 0;
}

// paladin_t::create_pets ====================================================

// FIXME: Not possible to check spec at this point, but in the future when all
// three versions of the guardian are implemented, it would be fugly to have to
// give them different names just for the lookup

void paladin_t::create_pets()
{
  guardian_of_ancient_kings = create_pet( "guardian_of_ancient_kings_ret" );
}

// paladin_t::combat_begin ===================================================

void paladin_t::combat_begin()
{
  player_t::combat_begin();

  if ( talents.communion -> rank() ) sim -> auras.communion -> trigger();
}

// paladin_t::holy_power_stacks ==============================================

int paladin_t::holy_power_stacks() SC_CONST
{
  if ( buffs_divine_purpose -> up() )
    return (int) resource_max[ RESOURCE_HOLY_POWER ];

   return (int) resource_current[ RESOURCE_HOLY_POWER ];
}

// paladin_t::get_divine_bulwark =============================================

double paladin_t::get_divine_bulwark() SC_CONST
{
  if ( primary_tree() != TREE_PROTECTION ) return 0.0;

  // block rating, 2.25% per point of mastery
  return composite_mastery() * passives.divine_bulwark -> base_value( E_APPLY_AURA, A_DUMMY );
}

// paladin_t::get_hand_of_light ==============================================

double paladin_t::get_hand_of_light() SC_CONST
{
  if ( primary_tree() != TREE_RETRIBUTION ) return 0.0;

  // chance to proc buff, 1% per point of mastery
  return composite_mastery() * passives.hand_of_light -> base_value( E_APPLY_AURA, A_DUMMY );
}

// ==========================================================================
// PLAYER_T EXTENSIONS
// ==========================================================================

player_t* player_t::create_paladin( sim_t* sim, const std::string& name, race_type r )
{
  return new paladin_t( sim, name, r );
}

// player_t::paladin_init ===================================================

void player_t::paladin_init( sim_t* sim )
{
  sim -> auras.communion     = new aura_t( sim, "communion",     1 );
  sim -> auras.devotion_aura = new aura_t( sim, "devotion_aura", 1 );

  for ( unsigned int i = 0; i < sim -> actor_list.size(); i++ )
  {
    player_t* p = sim -> actor_list[i];
    p -> buffs.blessing_of_kings        = new buff_t( p, "blessing_of_kings",       ! p -> is_pet() );
    p -> buffs.blessing_of_might        = new buff_t( p, "blessing_of_might",       ! p -> is_pet() );
    p -> buffs.blessing_of_might_regen  = new buff_t( p, "blessing_of_might_regen", ! p -> is_pet() );
    p -> debuffs.judgements_of_the_just = new debuff_t( p, "judgements_of_the_just", 1, 20.0 );
    p -> debuffs.vindication            = new debuff_t( p, "vindication",            1, 30.0 );
  }
}

// player_t::paladin_combat_begin ============================================

void player_t::paladin_combat_begin( sim_t* sim )
{
  double devo = sim -> dbc.effect_average( sim -> dbc.spell( 465 ) -> effect1 -> id(), sim -> max_player_level );
  double bow  = sim -> dbc.effect_average( sim -> dbc.spell( 79101 ) -> effect3 -> id(), sim -> max_player_level );

  if( sim -> overrides.communion     ) sim -> auras.communion     -> override();
  if( sim -> overrides.devotion_aura ) sim -> auras.devotion_aura -> override( 1, devo );

  for ( player_t* p = sim -> player_list; p; p = p -> next )
  {
    if ( p -> ooc_buffs() )
    {
      if ( sim -> overrides.blessing_of_kings )
        p -> buffs.blessing_of_kings -> override();
      if ( sim -> overrides.blessing_of_might )
      {
        p -> buffs.blessing_of_might       -> override();
        p -> buffs.blessing_of_might_regen -> override( 1, bow );
      }
    }
  }

  for ( target_t* t = sim -> target_list; t; t = t -> next )
  {
    if ( sim -> overrides.judgements_of_the_just ) t -> debuffs.judgements_of_the_just -> override();
    if ( sim -> overrides.vindication            ) t -> debuffs.vindication            -> override();
  }
}
